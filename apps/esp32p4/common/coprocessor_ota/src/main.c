// Updates the ESP32-C6 co-processor of a Guition ESP32-P4 panel (JC1060,
// JC8012) through esp_hosted over SDIO. The new co-processor firmware is read
// from the "coprocessor_fw" data partition of the P4's flash (written there by
// `make image`) and sent with esp_hosted_slave_ota_begin() / write() / end(),
// plus activate() on co-processor firmware 2.6 or newer; then the P4
// restarts. A co-processor already on the host's major.minor version is left
// alone. See README.md.
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "esp_event.h"
#include "esp_hosted.h"
#include "esp_hosted_ota.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

static const char* TAG = "coprocessor_ota";

enum
{
    kChunk         = 1500,
    kHeaderBytes   = 24,
    kSegmentHeader = 8,
    kHashBytes     = 32,
    kImageMagic    = 0xE9,
};

static uint8_t buffer[kChunk];

/// Size of the ESP application image at the start of partition: header,
/// segments, the checksum padded to 16 bytes and the appended SHA-256.
static bool image_size(const esp_partition_t* partition, uint32_t* size)
{
    uint8_t header[kHeaderBytes];
    if (esp_partition_read(partition, 0, header, sizeof(header)) != ESP_OK || header[0] != kImageMagic)
        return false;
    uint32_t offset = kHeaderBytes;
    for (uint8_t segment = 0; segment < header[1]; ++segment)
    {
        uint32_t segment_header[2];
        if (esp_partition_read(partition, offset, segment_header, sizeof(segment_header)) != ESP_OK)
            return false;
        offset += kSegmentHeader + segment_header[1];
        if (offset > partition->size)
            return false;
    }
    offset = (offset + 1 + 15) & ~15U;
    if (header[23] != 0)
        offset += kHashBytes;
    *size = offset;
    return offset <= partition->size;
}

static bool coprocessor_version(esp_hosted_coprocessor_fwver_t* version)
{
    if (esp_hosted_get_coprocessor_fwversion(version) != ESP_OK)
    {
        ESP_LOGW(TAG, "Co-processor version unknown");
        return false;
    }
    ESP_LOGI(TAG, "Co-processor firmware %" PRIu32 ".%" PRIu32 ".%" PRIu32 ", host %d.%d.%d", version->major1,
             version->minor1, version->patch1, ESP_HOSTED_VERSION_MAJOR_1, ESP_HOSTED_VERSION_MINOR_1,
             ESP_HOSTED_VERSION_PATCH_1);
    return true;
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Its own start-up constructor is not linked (see BRICK's EspWifi).
    ESP_ERROR_CHECK(esp_hosted_init());
    ESP_ERROR_CHECK(esp_hosted_connect_to_slave());

    esp_hosted_coprocessor_fwver_t before = { 0 };
    const bool                     known  = coprocessor_version(&before);
    if (known && before.major1 == ESP_HOSTED_VERSION_MAJOR_1 && before.minor1 == ESP_HOSTED_VERSION_MINOR_1)
    {
        ESP_LOGI(TAG, "RESULT: the co-processor matches the host, nothing to do");
        return;
    }

    const esp_partition_t* partition =
        esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "coprocessor_fw");
    uint32_t size = 0;
    if (partition == NULL || !image_size(partition, &size))
    {
        ESP_LOGE(TAG, "RESULT: no co-processor image in the coprocessor_fw partition (make image)");
        return;
    }

    ESP_LOGI(TAG, "Sending %" PRIu32 " bytes", size);
    esp_err_t error = esp_hosted_slave_ota_begin();
    if (error != ESP_OK)
    {
        ESP_LOGE(TAG, "RESULT: ota_begin failed: %s (nothing written)", esp_err_to_name(error));
        return;
    }
    for (uint32_t offset = 0; offset < size; offset += kChunk)
    {
        const uint32_t bytes = size - offset < kChunk ? size - offset : kChunk;
        ESP_ERROR_CHECK(esp_partition_read(partition, offset, buffer, bytes));
        error = esp_hosted_slave_ota_write(buffer, bytes);
        if (error != ESP_OK)
        {
            // The co-processor keeps running its old firmware.
            ESP_LOGE(TAG, "RESULT: ota_write at %" PRIu32 " failed: %s", offset, esp_err_to_name(error));
            return;
        }
        if (offset % (kChunk * 100) == 0)
            ESP_LOGI(TAG, "%" PRIu32 " / %" PRIu32, offset, size);
    }
    error = esp_hosted_slave_ota_end();
    if (error != ESP_OK)
    {
        ESP_LOGE(TAG, "RESULT: ota_end failed: %s", esp_err_to_name(error));
        return;
    }
    if (known && (before.major1 > 2 || (before.major1 == 2 && before.minor1 > 5)))
        ESP_LOGI(TAG, "ota_activate: %s", esp_err_to_name(esp_hosted_slave_ota_activate()));
    ESP_LOGI(TAG, "RESULT: firmware sent; restarting to resync with the co-processor");
    vTaskDelay(pdMS_TO_TICKS(3000));
    esp_restart();
}

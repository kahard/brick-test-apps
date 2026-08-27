#include <array>
#include <cstdint>
#include <cstdio>

#include "brick/core/storage/StorageWriteVerify.h"
#include "brick/platform/esp32/p4/SdmmcFileSystem.h"
#include "brick/platform/esp32/p4/profiles/guition_jc1060p470c_i_w.h"
#include "brick/platform/esp32/p4/MipiDsiDisplay.h"
#include "brick/interfaces/display/PixelBuffer.h"
#include "driver/gpio.h"

namespace {
constexpr std::uint16_t kWidth = 1024;
constexpr std::uint16_t kHeight = 600;
brick::platform::esp32::p4::MipiDsiDisplay display(
    brick::platform::esp32::p4::profiles::guition_jc1060p470c_i_w());
}

extern "C" void app_main()
{
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_23, 0);
    if (!display.begin()) {
        std::printf("DISPLAY INIT FAIL\n");
        return;
    }
    gpio_set_level(GPIO_NUM_23, 1);
    std::printf("DISPLAY READY\n");
    brick::platform::esp32::SdmmcFileSystem file_system;
    std::printf("SDMMC WRITE/READ VERIFY\n");
    if (!file_system.mount())
    {
        std::printf("SDMMC MOUNT FAIL\n");
        return;
    }
    std::array<std::uint8_t, 256> pattern{};
    for (std::size_t i = 0; i < pattern.size(); ++i)
        pattern[i] = static_cast<std::uint8_t>(i ^ 0x5AU);
    constexpr const char* kTestPath = "/sdcard/BRICK.BIN";
    const bool ok = brick::core::storage::write_verify(file_system,
                                                        kTestPath,
                                                        pattern.data(), pattern.size());
    std::printf("%s\n", ok ? "WRITE READ OK" : "WRITE READ FAIL");
    if (!ok)
    {
        auto diagnostic = file_system.open(kTestPath, "r");
        std::array<std::uint8_t, 256> readback{};
        const std::size_t read_count = diagnostic == nullptr ? 0U : diagnostic->read(readback.data(), 1U, readback.size());
        std::printf("DIAG open=%d read=%u first=%02X expected=%02X\n",
                    diagnostic != nullptr, static_cast<unsigned>(read_count), readback[0], pattern[0]);
    }
}

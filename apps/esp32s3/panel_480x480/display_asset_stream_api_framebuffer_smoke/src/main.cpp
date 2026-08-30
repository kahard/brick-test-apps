#include <array>
#include <cstdint>
#include <cstring>

#include "PartitionAssetSource.h"
#include "brick/core/image/AssetRepository.h"
#include "brick/core/image/AssetStreamer.h"
#include "brick/interfaces/display/IFrameBufferDisplay.h"
#include "brick/interfaces/display/ITouchscreen.h"
#include "brick/boards/esp32/s3/Panel480Board.h"
#include "esp_heap_caps.h"
#include "generated_assets.h"

namespace
{
constexpr char                       TAG[] = "brick_st7701s_asset_api_fb";
constexpr std::uint16_t              kWidth        = 480;
constexpr std::uint16_t              kHeight       = 480;
constexpr std::uint16_t              kStripeHeight = 20;
constexpr std::size_t                kScratchBytes = static_cast<std::size_t>(kWidth) * kStripeHeight * 2;

brick::platform::esp32::s3::Panel480Board board(
    brick::platform::esp32::s3::Panel480Board::double_buffered_config());
auto& display = board.display();
auto& touch = board.touch();
std::array<std::uint8_t, kScratchBytes>         scratch{};

PartitionAssetSource              asset_source("assets");
brick::core::image::AssetStreamer streamer(display);

class RamAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    explicit RamAssetSource(std::uint8_t* data) : data_(data) {}
    bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (data_ == nullptr || destination == nullptr || offset > asset.size || bytes > asset.size - offset)
            return false;
        std::memcpy(destination, data_ + asset.offset + offset, bytes);
        return true;
    }

private:
    std::uint8_t* data_;
};

class SdAssetSource final : public brick::interfaces::display::IAssetSource
{
public:
    SdAssetSource(brick::interfaces::storage::IFileSystem& filesystem, const char* path)
        : filesystem_(filesystem), path_(path)
    {
    }
    bool read(const brick::interfaces::display::AssetDescriptor& asset, std::size_t offset, std::uint8_t* destination,
              std::size_t bytes) override
    {
        if (destination == nullptr || offset > asset.size || bytes > asset.size - offset)
            return false;
        std::unique_ptr<brick::interfaces::storage::IFile> file = filesystem_.open(path_, "rb");
        if (!file || !file->seek(static_cast<long>(asset.offset + offset), SEEK_SET))
            return false;
        return file->read(destination, 1, bytes) == bytes;
    }

private:
    brick::interfaces::storage::IFileSystem& filesystem_;
    const char*                              path_;
};

std::uint8_t*                           psram_bundle = nullptr;
RamAssetSource                          psram_source(nullptr);
SdAssetSource                           sd_source(board.sd_card(), "/sdcard/ASSETS.BIN");
brick::core::image::AssetRepository     repository(&asset_source, &psram_source, &sd_source);

bool copy_to_psram()
{
    static bool copied = false;
    if (copied)
        return true;
    if (psram_bundle == nullptr)
    {
        psram_bundle = static_cast<std::uint8_t*>(heap_caps_malloc(generated_assets::bundle_size, MALLOC_CAP_SPIRAM));
        if (psram_bundle == nullptr)
        {
            board.logger().error(TAG, "Unable to allocate %u bytes in PSRAM",
                         static_cast<unsigned>(generated_assets::bundle_size));
            return false;
        }
        psram_source = RamAssetSource(psram_bundle);
    }
    for (std::size_t index = 0; index < generated_assets::entry_count; ++index)
    {
        const brick::interfaces::display::AssetDescriptor& asset = generated_assets::entries[index];
        if (!asset_source.read(asset, 0, psram_bundle + asset.offset, asset.size))
            return false;
    }
    copied = true;
    return true;
}
}  // namespace

extern "C" void app_main()
{
    board.logger().info(TAG, "Starting ESP32-S3 partition AssetStreamer framebuffer test: %ux%u pclk=12MHz", kWidth, kHeight);
    if (!display.begin())
    {
        board.logger().error(TAG, "ST7701S RGB display initialization failed");
        return;
    }
    if (!asset_source.begin())
    {
        board.logger().error(TAG, "Assets partition not found");
        return;
    }
    if (!touch.begin())
    {
        board.logger().error(TAG, "GT911 initialization failed");
        return;
    }

    auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(display);
    if (framebuffers.frame_buffer_count() != 2)
    {
        board.logger().error(TAG, "Expected two framebuffers, got %u", framebuffers.frame_buffer_count());
        return;
    }

    brick::interfaces::display::WritablePixelBuffer framebuffer[2];
    if (!framebuffers.get_frame_buffer(0, framebuffer[0]) || !framebuffers.get_frame_buffer(1, framebuffer[1]))
    {
        board.logger().error(TAG, "Unable to acquire both framebuffers");
        return;
    }

    const auto* initial_asset = generated_assets::get(generated_assets::Id::joy_tears);
    if (initial_asset == nullptr
        || !streamer.stream_to_buffer(*initial_asset, asset_source, framebuffer[0], scratch.data(), scratch.size())
        || !streamer.stream_to_buffer(*initial_asset, asset_source, framebuffer[1], scratch.data(), scratch.size())
        || !framebuffers.present_frame_buffer(0))
    {
        board.logger().error(TAG, "Unable to initialize asset framebuffer");
        return;
    }

    std::uint8_t  active            = 0;
    std::uint32_t frame             = 0;
    std::uint32_t benchmark_frames  = 0;
    std::uint64_t benchmark_started = board.time().micros();
    bool          background_mode   = false;
    std::uint8_t  storage_mode      = 0;
    bool          sd_ready          = false;
    bool          touch_down        = false;
    board.logger().info(TAG, "Partition AssetStreamer page-flip test active: touch toggles images/backgrounds");

    while (true)
    {
        const std::uint8_t                     back         = active ^ 1U;
        const bool                             second_asset = (frame & 1U) != 0U;
        brick::interfaces::display::TouchPoint point{};
        std::size_t                            touch_count = 0;
        const bool                             has_touch   = touch.read(&point, 1, touch_count) && touch_count > 0
                               && point.state != brick::interfaces::display::TouchState::released;
        if (has_touch && !touch_down)
        {
            const std::uint8_t state = static_cast<std::uint8_t>((storage_mode * 2U) + (background_mode ? 1U : 0U));
            const std::uint8_t next_state = static_cast<std::uint8_t>((state + 1U) % 6U);
            storage_mode                  = static_cast<std::uint8_t>(next_state / 2U);
            background_mode               = (next_state & 1U) != 0U;
            if (storage_mode == 1U && !copy_to_psram())
            {
                storage_mode = 0U;
                board.logger().warning(TAG, "PSRAM asset copy failed; falling back to flash");
            }
            if (storage_mode == 2U && !sd_ready)
            {
                sd_ready = board.sd_card().mount();
                if (!sd_ready)
                {
                    storage_mode = 0U;
                    board.logger().warning(TAG, "SD asset bundle unavailable; falling back to flash");
                }
            }
            const char* storage_name = storage_mode == 0U ? "flash" : (storage_mode == 1U ? "psram" : "sd");
            board.logger().info(TAG, "touch: storage=%s mode=%s x=%d y=%d", storage_name, background_mode ? "R/B" : "smiles",
                        point.x, point.y);
        }
        touch_down = has_touch;

        const auto selected_id =
            background_mode ?
                (second_asset ? generated_assets::Id::blue_background : generated_assets::Id::red_background) :
                (second_asset ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears);
        repository.set_storage(storage_mode == 0U ?
                                   brick::interfaces::display::AssetStorage::flash_partition :
                                   (storage_mode == 1U ? brick::interfaces::display::AssetStorage::psram_cache :
                                                         brick::interfaces::display::AssetStorage::usb_or_sd));
        brick::interfaces::display::IAssetSource*          selected_source = repository.source();
        const brick::interfaces::display::AssetDescriptor* asset           = generated_assets::get(selected_id);
        bool                                               frame_ok =
            asset != nullptr && selected_source != nullptr
            && streamer.stream_to_buffer(*asset, *selected_source, framebuffer[back], scratch.data(), scratch.size());
        if (!frame_ok && storage_mode == 2U)
        {
            board.logger().warning(TAG, "SD asset read failed; falling back to flash");
            board.sd_card().unmount();
            sd_ready     = false;
            storage_mode = 0U;
            repository.set_storage(brick::interfaces::display::AssetStorage::flash_partition);
            selected_source = repository.source();
            frame_ok        = asset != nullptr && selected_source != nullptr
                       && streamer.stream_to_buffer(*asset, *selected_source, framebuffer[back], scratch.data(),
                                                    scratch.size());
        }
        if (!frame_ok || !display.wait_for_vsync(100) || !framebuffers.present_frame_buffer(back))
        {
            board.logger().error(TAG, "Asset framebuffer page flip failed at frame=%u", static_cast<unsigned>(frame));
            return;
        }

        active = back;
        ++frame;
        ++benchmark_frames;
        // Detailed per-frame diagnostic (disabled to keep the UART readable):
        // board.logger().info(TAG, "presented frame=%u asset=%u storage=%u mode=%s flip=%lldus",
        //          static_cast<unsigned>(frame), static_cast<unsigned>(selected_id),
        //          static_cast<unsigned>(storage_mode),
        //          background_mode ? "backgrounds" : "smiles",
        //          board.time().micros() - frame_started);
        if (benchmark_frames == 60)
        {
            const auto elapsed = board.time().micros() - benchmark_started;
            board.logger().info(TAG, "asset framebuffer benchmark: frames=60 elapsed=%lldus fps=%.2f", elapsed,
                        60000000.0 / static_cast<double>(elapsed));
            benchmark_frames  = 0;
            benchmark_started = board.time().micros();
        }
    }
}


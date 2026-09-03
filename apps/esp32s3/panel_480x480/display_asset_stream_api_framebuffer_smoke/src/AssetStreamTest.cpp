#include "AssetStreamTest.h"

#include "AssetBenchmark.h"
#include "AssetPlayback.h"
#include "AssetSources.h"
#include "FramebufferPresenter.h"
#include "TouchInput.h"

namespace
{
constexpr char kTag[] = "brick_st7701s_asset_api_fb";

}  // namespace

struct AssetStreamTest::Impl
{
    explicit Impl(Board& board)
        : board(board), display(board.display()), touch(board.touch()), sources(board.sd_card()),
          presenter(display, display), touch_input(touch), playback(sources, presenter)
    {
    }

    bool initialize()
    {
        board.logger().info(kTag, "Starting partition AssetStreamer framebuffer test");
        if (!display.begin() || !touch.begin() || !sources.initialize())
            return false;
        if (!playback.initialize())
            return false;
        benchmark.reset();
        return true;
    }

    void update()
    {
        if (failed)
            return;
        if (touch_input.pressed())
        {
            const bool selected = playback.advance_mode();
            if (!selected)
                board.logger().warning(kTag, "Requested asset storage unavailable; using flash");
            board.logger().info(kTag, "touch: storage=%u mode=%s", playback.storage(),
                                playback.backgrounds() ? "R/B" : "smiles");
        }
        if (!playback.present_next())
        {
            board.logger().error(kTag, "Asset framebuffer presentation failed");
            failed = true;
            return;
        }
        benchmark.frame();
    }

    Board&                                           board;
    brick::platform::esp32::s3::St7701sRgbDisplay&   display;
    brick::platform::esp32::touch::Gt911Touchscreen& touch;
    AssetSources                                     sources;
    FramebufferPresenter                             presenter;
    TouchInput                                       touch_input;
    AssetPlayback                                    playback;
    AssetBenchmark                                   benchmark{ board.time(), board.logger() };
    bool                                             failed = false;
};

AssetStreamTest::AssetStreamTest(Board& board) : impl_(std::make_unique<Impl>(board))
{
}
AssetStreamTest::~AssetStreamTest() = default;
bool AssetStreamTest::initialize()
{
    return impl_->initialize();
}
void AssetStreamTest::update()
{
    impl_->update();
}

#include "FramebufferTest.h"

#include "brick/interfaces/display/IFrameBufferDisplay.h"

namespace
{
constexpr char          TAG[]   = "brick_st7701s_framebuffer";
constexpr std::uint16_t kWidth  = 480;
constexpr std::uint16_t kHeight = 480;
}  // namespace

FramebufferTest::FramebufferTest(brick::platform::esp32::s3::Panel480BoardTemplate<FramebufferFeatures>& board)
    : board_(board), display_(&board.display())
{
}

bool FramebufferTest::initialize()
{
    board_.logger().info(TAG, "Starting ESP32-S3 ST7701S framebuffer smoke test");
    if (!display_->begin())
        return false;
    auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(*display_);
    board_.logger().info(TAG, "Display initialized, framebuffer count=%u", framebuffers.frame_buffer_count());
    return framebuffers.frame_buffer_count() >= 2;
}

bool FramebufferTest::fill_and_present(std::uint8_t index, std::uint16_t color)
{
    auto& framebuffers = static_cast<brick::interfaces::display::IFrameBufferDisplay&>(*display_);
    brick::interfaces::display::WritablePixelBuffer buffer;
    if (!framebuffers.get_frame_buffer(index, buffer))
        return false;
    const std::uint64_t start  = board_.time().micros();
    auto*               pixels = reinterpret_cast<std::uint16_t*>(buffer.data);
    for (std::size_t pixel = 0; pixel < kWidth * kHeight; ++pixel)
        pixels[pixel] = color;
    const std::uint64_t fill = board_.time().micros() - start;
    if (!framebuffers.present_frame_buffer(index) || !display_->wait_for_vsync(100))
        return false;
    board_.logger().info(TAG, "framebuffer=%u color=0x%04X fill=%lldus present+vsync=%lldus", index, color,
                         static_cast<long long>(fill), static_cast<long long>(board_.time().micros() - start));
    return true;
}

void FramebufferTest::update()
{
    static std::uint8_t index = 0;
    fill_and_present(index, index == 0 ? 0xF800 : 0x001F);
    index ^= 1U;
    board_.time().delay_ms(500);
}

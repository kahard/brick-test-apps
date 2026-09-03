#pragma once

#include "ApplicationConfig.h"
#include "brick/boards/esp32/s3/Panel480BoardTemplate.h"

class FramebufferTest final
{
public:
    explicit FramebufferTest(brick::platform::esp32::s3::Panel480BoardTemplate<FramebufferFeatures>& board);
    bool initialize();
    void update();

private:
    bool fill_and_present(std::uint8_t index, std::uint16_t color);
    brick::platform::esp32::s3::Panel480BoardTemplate<FramebufferFeatures>& board_;
    brick::platform::esp32::s3::St7701sRgbDisplay*                          display_;
};

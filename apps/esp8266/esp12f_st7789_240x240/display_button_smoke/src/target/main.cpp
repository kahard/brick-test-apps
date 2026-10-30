// ESP-12F entry point: builds the board, hands its peripherals to the
// portable application and runs it from the Arduino loop.
#include <Arduino.h>

#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp8266/Esp12fSt7789Board.h"

namespace
{
using Board = brick::boards::Esp12fSt7789BoardTemplate<esp12f_st7789_button_smoke::Features>;

esp12f_st7789_button_smoke::Application* application = nullptr;
}  // namespace

void setup()
{
    Serial.begin(115200);
    delay(50);
    static Board board;
    if (!board.begin())
    {
        board.logger().error("display_button_smoke", "board initialization failed");
        return;
    }
    static esp12f_st7789_button_smoke::Application instance(
        { board.display(), board.button(), board.time(), board.logger() });
    if (instance.initialize())
        application = &instance;
}

void loop()
{
    if (application != nullptr)
        application->update();
}

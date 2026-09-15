#include <Arduino.h>

#include "Application.h"

namespace
{
esp12f_st7789_asset_stream_smoke::Application application;
}

void setup()
{
    Serial.begin(115200);
    delay(50);
    application.initialize();
}

void loop()
{
    application.update();
}

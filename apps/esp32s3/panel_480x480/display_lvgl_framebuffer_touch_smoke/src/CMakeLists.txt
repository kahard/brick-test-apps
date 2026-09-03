idf_component_register(
    SRCS
        "main.cpp"
        "Application.cpp"
        "LvglTest.cpp"
        "fonts/brick_font_24.c"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglDisplayAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglTouchAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
        "../../../../../libs/brick/platforms/esp32/src/FreeRtosTime.cpp"
        "../../../../../libs/brick/platforms/esp32/src/EspIdfLogger.cpp"
    INCLUDE_DIRS "."
)

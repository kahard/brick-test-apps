idf_component_register(
    SRCS
        "main.cpp"
        "fonts/brick_font_24.c"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglDisplayAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglTouchAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "."
)

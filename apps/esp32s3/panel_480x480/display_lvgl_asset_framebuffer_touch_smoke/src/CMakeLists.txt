idf_component_register(
    SRCS
        "main.cpp"
        "fonts/brick_roboto_24.c"
        "images/sweat_smile_rgb565.c"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglDisplayAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/LvglTouchAdapter.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "."
    EMBED_FILES
        "../assets/generated/joy_tears_rgb565.bin"
        "../assets/generated/joy_tears_stream_rgb565.bin"
)

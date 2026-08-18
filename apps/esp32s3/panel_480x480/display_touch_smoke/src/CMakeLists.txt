idf_component_register(
    SRCS
        "main.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
    INCLUDE_DIRS "."
)

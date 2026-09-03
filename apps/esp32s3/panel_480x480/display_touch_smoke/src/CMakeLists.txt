idf_component_register(
    SRCS
        "main.cpp"
        "Application.cpp"
        "TouchTest.cpp"
        "ColorPattern.cpp"
        "TouchMarker.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "../../../../../libs/brick/platforms/esp32/src/FreeRtosTime.cpp"
        "../../../../../libs/brick/platforms/esp32/src/EspIdfLogger.cpp"
        "../../../../../libs/brick/platforms/esp32/src/SdSpiFileSystem.cpp"
        "../../../../../libs/brick/platforms/esp32/src/File.cpp"
    INCLUDE_DIRS "."
)

idf_component_register(
    SRCS
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "../../../../../libs/brick/libs/core/src/AssetBundle.cpp"
        "../../../../../libs/brick/libs/core/src/AssetStreamer.cpp"
        "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
        "../../../../../libs/brick/platforms/esp32/src/SdSpiFileSystem.cpp"
        "../../../../../libs/brick/platforms/esp32/src/EspIdfLogger.cpp"
        "../../../../../libs/brick/platforms/esp32/src/File.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
        "main.cpp"
    INCLUDE_DIRS "."
)

idf_component_register(
    SRCS
        "main.cpp"
        "../../../../../libs/brick/platforms/esp32/src/St7701sRgbDisplay.cpp"
          "../../../../../libs/brick/platforms/esp32/src/Gt911Touchscreen.cpp"
          "../../../../../libs/brick/platforms/esp32/src/File.cpp"
          "../../../../../libs/brick/platforms/esp32/src/SdSpiFileSystem.cpp"
          "../../../../../libs/brick/platforms/esp32/src/File.cpp"
          "../../../../../libs/brick/platforms/esp32/src/SdSpiFileSystem.cpp"
        "../../../../../libs/brick/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "."
)

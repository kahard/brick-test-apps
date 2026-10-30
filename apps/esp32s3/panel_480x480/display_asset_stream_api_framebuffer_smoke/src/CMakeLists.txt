set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
idf_component_register(
    SRCS
        "target/main.cpp"
        "app/Application.cpp"
        "app/AssetBenchmark.cpp"
        "app/AssetPlayback.cpp"
        "app/AssetSources.cpp"
        "app/AssetStreamTest.cpp"
        "app/FramebufferPresenter.cpp"
        "app/TouchInput.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "${BRICK_ROOT}/libs/core/src/AssetBundle.cpp"
        "${BRICK_ROOT}/libs/core/src/AssetStreamer.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gt911Touchscreen.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/SdSpiFileSystem.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/PartitionAssetSource.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/File.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "." "app" "../assets/generated"
)

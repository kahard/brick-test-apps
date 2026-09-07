set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
set(app_sources
    "PixelMemory.cpp"
    "FrameBuffers.cpp"
    "main.cpp"
    "LvglRuntime.cpp"
    "LvglAssets.cpp"
    "BenchmarkSeries.cpp"
    "LvglTest.cpp"
    "Application.cpp"
)
# Compile the board's implementation set; the template and linker discard
# unused peripherals. ApplicationConfig.h remains the single feature switch.
idf_component_register(
    SRCS ${app_sources}
        "${BRICK_ROOT}/platforms/esp32/src/MipiDsiDisplay.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gt911Touchscreen.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/SdmmcFileSystem.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/PartitionAssetSource.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/LvglDisplayAdapter.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "." "../generated"
)

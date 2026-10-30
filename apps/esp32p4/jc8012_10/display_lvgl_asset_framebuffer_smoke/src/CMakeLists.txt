set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
# Compile the board's implementation set; the template and linker discard
# unused peripherals. ApplicationConfig.h remains the single feature switch.
idf_component_register(
    SRCS
        "target/main.cpp"
        "app/Application.cpp"
        "app/FrameBuffers.cpp"
        "app/LvglAssets.cpp"
        "app/LvglRuntime.cpp"
        "app/LvglTest.cpp"
        "app/TouchView.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/MipiDsiDisplay.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gsl3680Touchscreen.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/PartitionAssetSource.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglDisplayAdapter.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglTouchAdapter.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "." "app" "../generated"
)

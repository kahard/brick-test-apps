set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../../libs/brick")
# The benchmark itself is shared by every board: apps/shared/benchmark.
set(BENCHMARK_SHARED "${CMAKE_CURRENT_LIST_DIR}/../../../../../shared/benchmark")
file(GLOB benchmark_sources "${BENCHMARK_SHARED}/*.cpp")
idf_component_register(
    SRCS
        "target/main.cpp"
        ${benchmark_sources}
        "${BRICK_ROOT}/platforms/esp32/src/Ili9341SpiDisplay.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Xpt2046Touchscreen.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglDisplayAdapter.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglTouchAdapter.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
    INCLUDE_DIRS "." "app" "${BENCHMARK_SHARED}"
)

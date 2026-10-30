set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
idf_component_register(
    SRCS
        "target/main.cpp"
        "app/Application.cpp"
        "app/LvglTest.cpp"
        "app/fonts/brick_font_24.c"
        "${BRICK_ROOT}/platforms/esp32/src/St7701sRgbDisplay.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglDisplayAdapter.cpp"
        "${BRICK_ROOT}/libs/lvgl/src/LvglTouchAdapter.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gt911Touchscreen.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
    INCLUDE_DIRS "." "app"
)

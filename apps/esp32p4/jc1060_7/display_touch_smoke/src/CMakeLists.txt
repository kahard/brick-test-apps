set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
idf_component_register(
    SRCS
        "target/main.cpp"
        "app/Application.cpp"
        "app/TouchTest.cpp"
        "app/TouchMarker.cpp"
        "app/ColorPattern.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/MipiDsiDisplay.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gt911Touchscreen.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/SdmmcFileSystem.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "." "app"
)

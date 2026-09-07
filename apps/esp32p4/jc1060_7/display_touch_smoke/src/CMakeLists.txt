set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
idf_component_register(
    SRCS
        "main.cpp"
        "Application.cpp"
        "TouchTest.cpp"
        "TouchMarker.cpp"
        "ColorPattern.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/MipiDsiDisplay.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/Gt911Touchscreen.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/SdmmcFileSystem.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
        "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
        "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp"
    INCLUDE_DIRS "."
)

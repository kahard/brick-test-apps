set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick")
file(GLOB_RECURSE app_sources "${CMAKE_CURRENT_LIST_DIR}/app/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/target/*.cpp")
list(APPEND app_sources
    "${BRICK_ROOT}/platforms/esp32/src/MipiDsiDisplay.cpp"
    "${BRICK_ROOT}/platforms/esp32/src/Gsl3680Touchscreen.cpp"
    "${BRICK_ROOT}/platforms/esp32/src/FreeRtosTime.cpp"
    "${BRICK_ROOT}/platforms/esp32/src/EspIdfLogger.cpp"
    "${BRICK_ROOT}/libs/core/src/TouchMapper.cpp")
idf_component_register(SRCS ${app_sources} INCLUDE_DIRS "." "app")

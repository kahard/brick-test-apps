file(GLOB app_sources "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
list(APPEND app_sources
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/platforms/esp32/src/MipiDsiDisplay.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/platforms/esp32/src/Gsl3680Touchscreen.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/platforms/esp32/src/SdmmcFileSystem.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/platforms/esp32/src/FreeRtosTime.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/platforms/esp32/src/EspIdfLogger.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../../../../libs/brick/libs/core/src/TouchMapper.cpp")
idf_component_register(SRCS ${app_sources} INCLUDE_DIRS ".")

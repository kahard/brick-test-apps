set(network_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../../shared/network")
brick_app_simulator(cyd_network_wifi_internet_smoke PREBUILD font FRAMES 400
  SOURCES "${network_shared}/InternetTest.cpp" "${network_shared}/InternetTestScreen.cpp"
          "${network_shared}/TextLines.cpp"
  SMOKE_ARGS --offline --touch 160,120@4000+100)
target_include_directories(cyd_network_wifi_internet_smoke PRIVATE "${network_shared}")

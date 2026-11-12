# The application and target sources; BRICK and the shared audio test are
# added by extra_script.py (tools/audio_app_build.py).
file(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)

idf_component_register(SRCS ${app_sources})

# PC simulator builds of the test applications.
#
# Every application with a pc/simulator.cmake is built here against
# BRICK::PlatformPC and registered as a headless smoke test in CTest.
# Use `make pc` / `make pc-test` from the repository root (tools/pc.py).
cmake_minimum_required(VERSION 3.20)
project(brick_test_apps_pc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

# BRICK_ROOT may point at a framework checkout under development.
set(BRICK_ROOT "${CMAKE_CURRENT_LIST_DIR}/../libs/brick" CACHE PATH "BRICK framework sources")
add_subdirectory("${BRICK_ROOT}" brick)

# GNU make runs an application's own generators (fonts, assets) before its
# simulator is compiled, exactly as for the embedded build.
find_program(BRICK_MAKE NAMES make gmake mingw32-make)

# LVGL for applications that use it, at the version pinned for the embedded
# builds in platformio-versions.ini, with the same default configuration
# (LV_CONF_SKIP). Fonts the applications enable on the target are all built in.
function(brick_pc_lvgl)
  if(TARGET brick_pc_lvgl)
    return()
  endif()
  file(STRINGS "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../platformio-versions.ini" lvgl_pin REGEX "^lvgl *=")
  string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" lvgl_version "${lvgl_pin}")
  if(NOT lvgl_version)
    message(FATAL_ERROR "No lvgl version in platformio-versions.ini")
  endif()
  include(FetchContent)
  # SOURCE_SUBDIR points nowhere so LVGL's own CMake project is not configured.
  FetchContent_Declare(lvgl
    GIT_REPOSITORY https://github.com/lvgl/lvgl.git
    GIT_TAG v${lvgl_version}
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR brick-no-cmake)
  FetchContent_MakeAvailable(lvgl)
  file(GLOB_RECURSE lvgl_sources CONFIGURE_DEPENDS "${lvgl_SOURCE_DIR}/src/*.c")
  add_library(brick_pc_lvgl STATIC ${lvgl_sources})
  target_include_directories(brick_pc_lvgl SYSTEM PUBLIC "${lvgl_SOURCE_DIR}")
  set(fonts)
  foreach(size 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48)
    list(APPEND fonts LV_FONT_MONTSERRAT_${size}=1)
  endforeach()
  target_compile_definitions(brick_pc_lvgl PUBLIC LV_CONF_SKIP=1 ${fonts})
  message(STATUS "BRICK test apps: LVGL ${lvgl_version} for the simulators")
endfunction()

# brick_app_simulator(<name> [LVGL] [FRAMES n] [DEFINES name=value...]
#                     [PREBUILD make-targets...] [SMOKE_ARGS args...] [SOURCES files...])
#
# Call from an application's pc/simulator.cmake. Builds every .cpp/.c under
# src/app plus pc/main.cpp (and SOURCES) into <name> and adds a CTest that runs
# it headless with deterministic time for FRAMES updates (default 120),
# passing SMOKE_ARGS (scripted touches, presses) and saving a screenshot.
# PREBUILD names targets of the application's Makefile (e.g. fonts) to run
# first. LVGL links LVGL and the BRICK LVGL adapters.
function(brick_app_simulator name)
  cmake_parse_arguments(ARG "LVGL" "FRAMES" "DEFINES;PREBUILD;SMOKE_ARGS;SOURCES" ${ARGN})
  set(pc_dir "${CMAKE_CURRENT_LIST_DIR}")
  get_filename_component(app_dir "${pc_dir}/.." ABSOLUTE)
  # Generated sources (e.g. LVGL fonts in src/app/fonts) must exist before the
  # source list is collected, so PREBUILD also runs here at configure time.
  if(ARG_PREBUILD)
    if(NOT BRICK_MAKE)
      message(FATAL_ERROR "${name} needs GNU make to run: ${ARG_PREBUILD}")
    endif()
    execute_process(COMMAND "${BRICK_MAKE}" -C "${app_dir}" ${ARG_PREBUILD}
                    RESULT_VARIABLE prebuild_result OUTPUT_QUIET)
    if(NOT prebuild_result EQUAL 0)
      message(FATAL_ERROR "${name}: make ${ARG_PREBUILD} failed in ${app_dir}")
    endif()
  endif()
  file(GLOB_RECURSE app_sources CONFIGURE_DEPENDS "${app_dir}/src/app/*.cpp" "${app_dir}/src/app/*.c")
  brick_add_simulator(${name} ${app_sources} "${pc_dir}/main.cpp" ${ARG_SOURCES})
  target_include_directories(${name} PRIVATE "${app_dir}/src/app")
  # Values the embedded build passes with -D in platformio.ini.
  target_compile_definitions(${name} PRIVATE ${ARG_DEFINES})
  if(ARG_LVGL)
    brick_pc_lvgl()
    target_link_libraries(${name} PRIVATE brick_pc_lvgl BRICK::Lvgl)
  endif()
  if(ARG_PREBUILD)
    # And again before every build, so edited sources are regenerated.
    add_custom_target(${name}_prebuild
      COMMAND "${BRICK_MAKE}" -C "${app_dir}" ${ARG_PREBUILD}
      COMMENT "Generating sources for ${name}"
      VERBATIM)
    add_dependencies(${name} ${name}_prebuild)
  endif()
  target_compile_options(${name} PRIVATE -Wall -Wextra)
  if(NOT ARG_FRAMES)
    set(ARG_FRAMES 120)
  endif()
  # Every run gets its own empty directory as the SD card.
  set(card "${CMAKE_BINARY_DIR}/sdcard/${name}")
  file(MAKE_DIRECTORY "${card}")
  add_test(NAME ${name}
           COMMAND ${name} --headless --manual-time --frames ${ARG_FRAMES} --fs "${card}"
                   --screenshot "${CMAKE_BINARY_DIR}/screenshots/${name}.ppm" ${ARG_SMOKE_ARGS})
endfunction()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/screenshots")
file(GLOB_RECURSE simulators CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../apps/*simulator.cmake")
list(FILTER simulators INCLUDE REGEX "/pc/simulator\\.cmake$")
list(FILTER simulators EXCLUDE REGEX "/\\.pio/")
list(SORT simulators)
foreach(simulator IN LISTS simulators)
  include("${simulator}")
endforeach()
list(LENGTH simulators count)
message(STATUS "BRICK test apps: ${count} PC simulator(s)")

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

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../libs/brick" brick)

# GNU make runs an application's own generators (fonts, assets) before its
# simulator is compiled, exactly as for the embedded build.
find_program(BRICK_MAKE NAMES make gmake mingw32-make)

# brick_app_simulator(<name> [FRAMES n] [DEFINES name=value...] [PREBUILD make-targets...]
#                     [SMOKE_ARGS args...] [SOURCES files...])
#
# Call from an application's pc/simulator.cmake. Builds every .cpp/.c under
# src/app plus pc/main.cpp (and SOURCES) into <name> and adds a CTest that runs
# it headless with deterministic time for FRAMES updates (default 120),
# passing SMOKE_ARGS (scripted touches, presses) and saving a screenshot.
# PREBUILD names targets of the application's Makefile (e.g. fonts) to run
# first.
function(brick_app_simulator name)
  cmake_parse_arguments(ARG "" "FRAMES" "DEFINES;PREBUILD;SMOKE_ARGS;SOURCES" ${ARGN})
  set(pc_dir "${CMAKE_CURRENT_LIST_DIR}")
  get_filename_component(app_dir "${pc_dir}/.." ABSOLUTE)
  file(GLOB_RECURSE app_sources CONFIGURE_DEPENDS "${app_dir}/src/app/*.cpp" "${app_dir}/src/app/*.c")
  brick_add_simulator(${name} ${app_sources} "${pc_dir}/main.cpp" ${ARG_SOURCES})
  target_include_directories(${name} PRIVATE "${app_dir}/src/app")
  # Values the embedded build passes with -D in platformio.ini.
  target_compile_definitions(${name} PRIVATE ${ARG_DEFINES})
  if(ARG_PREBUILD)
    if(NOT BRICK_MAKE)
      message(FATAL_ERROR "${name} needs GNU make to run: ${ARG_PREBUILD}")
    endif()
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
  add_test(NAME ${name}
           COMMAND ${name} --headless --manual-time --frames ${ARG_FRAMES}
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

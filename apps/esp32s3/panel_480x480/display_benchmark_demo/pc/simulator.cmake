# The shared benchmark (apps/shared/benchmark) plus this board's entry point.
# Each update advances 16 ms; the touches skip through the six scenes, so the
# screenshot shows the summary table.
set(benchmark_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../shared/benchmark")
file(GLOB benchmark_sources CONFIGURE_DEPENDS "${benchmark_shared}/*.cpp")
brick_app_simulator(panel480_display_benchmark_demo LVGL FRAMES 500
  SOURCES ${benchmark_sources}
  SMOKE_ARGS --touch 240,240@1000+100 --touch 240,240@2000+100 --touch 240,240@3000+100
             --touch 240,240@4000+100 --touch 240,240@5000+100 --touch 240,240@6000+100)
target_include_directories(panel480_display_benchmark_demo PRIVATE "${benchmark_shared}")

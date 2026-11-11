# The shared benchmark (apps/shared/benchmark) plus this board's entry point.
# Each update advances 16 ms; the touches skip through the six scenes, so the
# screenshot shows the summary table.
set(benchmark_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../shared/benchmark")
file(GLOB benchmark_sources CONFIGURE_DEPENDS "${benchmark_shared}/*.cpp")
brick_app_simulator(jc1060_display_benchmark_demo LVGL FRAMES 500
  SOURCES ${benchmark_sources}
  SMOKE_ARGS --touch 512,300@1000+100 --touch 512,300@2000+100 --touch 512,300@3000+100
             --touch 512,300@4000+100 --touch 512,300@5000+100 --touch 512,300@6000+100)
target_include_directories(jc1060_display_benchmark_demo PRIVATE "${benchmark_shared}")

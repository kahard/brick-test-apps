# The shared benchmark (apps/shared/benchmark) plus this board's entry point.
# Each update advances 16 ms; the touches skip through the six scenes, so the
# screenshot shows the summary table.
set(benchmark_shared "${CMAKE_CURRENT_LIST_DIR}/../../../../shared/benchmark")
file(GLOB benchmark_sources CONFIGURE_DEPENDS "${benchmark_shared}/*.cpp")
brick_app_simulator(jc8012_display_benchmark_demo LVGL FRAMES 500
  SOURCES ${benchmark_sources}
  SMOKE_ARGS --touch 400,640@1000+100 --touch 400,640@2000+100 --touch 400,640@3000+100
             --touch 400,640@4000+100 --touch 400,640@5000+100 --touch 400,640@6000+100)
target_include_directories(jc8012_display_benchmark_demo PRIVATE "${benchmark_shared}")

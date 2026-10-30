# `make assets` writes generated/assets.bin, which the embedded build flashes
# to the asset partition, and generated/generated_assets.h.
# 200 updates cover the first timed series of 60 frames.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../generated")
brick_app_simulator(jc8012_display_lvgl_benchmark_smoke
  LVGL
  FRAMES 200
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\"")
target_include_directories(jc8012_display_lvgl_benchmark_smoke PRIVATE "${asset_dir}")

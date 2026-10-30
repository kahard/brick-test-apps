# `make assets` writes assets/generated/assets.bin, which is flashed to the
# asset partition, and assets/generated/generated_assets.h.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../assets/generated")
brick_app_simulator(cyd_display_asset_stream_api_framebuffer_smoke
  FRAMES 90
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 160,120@300+50 --touch 160,120@600+50 --touch 160,120@900+50 --touch 160,120@1200+50)
target_include_directories(cyd_display_asset_stream_api_framebuffer_smoke PRIVATE "${asset_dir}")

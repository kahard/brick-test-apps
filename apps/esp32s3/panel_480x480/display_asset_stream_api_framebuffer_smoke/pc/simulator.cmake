# `make assets` writes assets/generated/assets.bin, which is flashed to the
# asset partition, and assets/generated/generated_assets.h.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../assets/generated")
brick_app_simulator(panel480_display_asset_stream_api_framebuffer_smoke
  FRAMES 90
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 240,240@300+50 --touch 240,240@600+50 --touch 240,240@900+50 --touch 240,240@1200+50)
target_include_directories(panel480_display_asset_stream_api_framebuffer_smoke PRIVATE "${asset_dir}")

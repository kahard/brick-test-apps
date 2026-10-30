# `make assets` writes generated/assets.bin, which the embedded build flashes
# to the asset partition, and generated/generated_assets.h.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../generated")
brick_app_simulator(jc1060_display_asset_stream_api_framebuffer_smoke
  FRAMES 90
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 512,300@300+50 --touch 512,300@600+50 --touch 512,300@900+50 --touch 512,300@1200+50)
target_include_directories(jc1060_display_asset_stream_api_framebuffer_smoke PRIVATE "${asset_dir}")

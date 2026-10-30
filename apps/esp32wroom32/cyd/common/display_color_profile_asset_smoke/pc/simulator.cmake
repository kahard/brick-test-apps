# `make assets` writes assets/generated/assets.bin, which is flashed to the
# asset partition, and assets/generated/generated_assets.h. The scripted touch
# drags the divider from the center to x=240.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../assets/generated")
brick_app_simulator(cyd_display_color_profile_asset_smoke
  FRAMES 60
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 160,120@200+100 --touch 200,120@300+100 --touch 240,120@400+100)
target_include_directories(cyd_display_color_profile_asset_smoke PRIVATE "${asset_dir}")

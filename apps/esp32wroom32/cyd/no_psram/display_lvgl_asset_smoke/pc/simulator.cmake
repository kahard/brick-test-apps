# `make assets` generates the font in src/app/fonts and assets/generated with
# assets.bin (flashed to the asset partition) and generated_assets.h. The
# scripted touch presses the button once, switching to the second image.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../assets/generated")
brick_app_simulator(cyd_display_lvgl_asset_smoke
  LVGL
  FRAMES 60
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 160,187@300+100)
target_include_directories(cyd_display_lvgl_asset_smoke PRIVATE "${asset_dir}")

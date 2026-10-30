# `make assets` writes generated/assets.bin, which the embedded build flashes
# to the asset partition, and generated/generated_assets.h.
# The scripted touch clicks the button once, switching to the second image.
set(asset_dir "${CMAKE_CURRENT_LIST_DIR}/../generated")
brick_app_simulator(jc8012_display_lvgl_asset_framebuffer_smoke
  LVGL
  FRAMES 60
  PREBUILD assets
  DEFINES "BRICK_PC_ASSET_BUNDLE=\"${asset_dir}/assets.bin\""
  SMOKE_ARGS --touch 400,1210@300+100)
target_include_directories(jc8012_display_lvgl_asset_framebuffer_smoke PRIVATE "${asset_dir}")

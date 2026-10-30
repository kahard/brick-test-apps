# The scripted touch clicks the centered button twice.
brick_app_simulator(panel480_display_lvgl_framebuffer_touch_smoke
  LVGL
  FRAMES 90
  SMOKE_ARGS --touch 240,240@300+100 --touch 240,240@700+100)

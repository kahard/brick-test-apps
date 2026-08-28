from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[3] / "libs" / "brick"
lvgl_root = project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl"
touch_core = project_dir / "managed_components" / "espressif__esp_lcd_touch"
touch_gsl = project_dir / "components" / "esp_lcd_touch_gsl3680"
touch_enabled = (
    "BRICK_LVGL_TOUCH" in env.subst("$BUILD_FLAGS")
    or any("BRICK_LVGL_TOUCH" in str(define) for define in env.get("CPPDEFINES", []))
)

include_paths = [
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "libs" / "core" / "include"),
    str(brick_root / "platforms" / "esp32" / "include"),
    str(project_dir.parents[3] / "libs" / "brick-boards" / "include"),
    str(lvgl_root / "src"),
]
if touch_enabled:
    include_paths.extend([str(touch_core / "include"), str(touch_gsl / "include")])
env.Append(CPPPATH=include_paths)

brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(str(brick_build_dir / "display"), str(brick_root / "platforms" / "esp32" / "src"), "+<MipiDsiDisplay.cpp>")
env.BuildSources(str(brick_build_dir / "lvgl"), str(brick_root / "platforms" / "esp32" / "src"), "+<LvglDisplayAdapter.cpp>")
env.BuildSources(str(brick_build_dir / "lvgl"), str(brick_root / "platforms" / "esp32" / "src"), "+<LvglTouchAdapter.cpp>")
env.BuildSources(str(brick_build_dir / "core"), str(brick_root / "libs" / "core" / "src"), "+<TouchMapper.cpp>")
env.BuildSources(str(brick_build_dir / "core"), str(brick_root / "libs" / "core" / "src"), "+<AssetStreamer.cpp>")
if touch_enabled:
    env.BuildSources(str(brick_build_dir / "touch"), str(brick_root / "platforms" / "esp32" / "src"), "+<Gsl3680Touchscreen.cpp>")
    env.BuildSources(str(brick_build_dir / "touch_core"), str(touch_core), "+<esp_lcd_touch.c>")
    env.BuildSources(str(brick_build_dir / "touch_gsl"), str(touch_gsl), "+<esp_lcd_touch_gsl3680.c>")
    env.BuildSources(str(brick_build_dir / "touch_gsl"), str(touch_gsl), "+<gsl_point_id.c>")

from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[4] / "libs" / "brick"
env.Append(CPPPATH=[str(brick_root / "libs" / "interfaces" / "include"), str(brick_root / "libs" / "core" / "include"), str(brick_root / "platforms" / "esp32" / "include")])
env.Append(CPPPATH=[str(project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl" / "src")])
brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(str(brick_build_dir / "lvgl"), str(brick_root / "platforms" / "esp32" / "src"), "+<LvglTouchAdapter.cpp>")

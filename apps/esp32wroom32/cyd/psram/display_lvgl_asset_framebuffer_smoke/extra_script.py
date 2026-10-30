from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[4] / "libs" / "brick"
boards_root = brick_root / "boards"
env.Append(CPPPATH=[str(brick_root / "libs" / "interfaces" / "include"), str(brick_root / "libs" / "core" / "include"), str(brick_root / "platforms" / "esp32" / "include"), str(brick_root / "libs" / "lvgl" / "include"), str(boards_root / "include")])
env.Append(CPPPATH=[str(project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl" / "src")])
brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(str(brick_build_dir / "core"), str(brick_root / "libs" / "core" / "src"), "+<TouchMapper.cpp>")
env.BuildSources(str(brick_build_dir / "platform"), str(brick_root / "platforms" / "esp32" / "src"), "+<Ili9341SpiDisplay.cpp>  +<Xpt2046Touchscreen.cpp> +<FreeRtosTime.cpp> +<EspIdfLogger.cpp> +<PartitionAssetSource.cpp>")
env.BuildSources(str(brick_build_dir / "lvgl"), str(brick_root / "libs" / "lvgl" / "src"), "+<LvglDisplayAdapter.cpp> +<LvglTouchAdapter.cpp>")

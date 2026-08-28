from pathlib import Path
Import("env")
project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[3] / "libs" / "brick"
env.Append(CPPPATH=[str(brick_root / "libs" / "interfaces" / "include"), str(brick_root / "libs" / "core" / "include"), str(brick_root / "platforms" / "esp32" / "include"), str(project_dir.parents[3] / "libs" / "brick-boards" / "include")])
env.Append(CPPPATH=[str(project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl" / "src")])
brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(str(brick_build_dir / "core"), str(brick_root / "libs" / "core" / "src"), "+<TouchMapper.cpp>")
env.BuildSources(str(brick_build_dir / "touch"), str(brick_root / "platforms" / "esp32" / "src"), "+<Gt911Touchscreen.cpp>")
env.BuildSources(str(brick_build_dir / "display"), str(brick_root / "platforms" / "esp32" / "src"), "+<MipiDsiDisplay.cpp>")
env.BuildSources(str(brick_build_dir / "storage"), str(brick_root / "platforms" / "esp32" / "src"), "+<SdmmcFileSystem.cpp>")
env.BuildSources(str(brick_build_dir / "lvgl"), str(brick_root / "platforms" / "esp32" / "src"), "+<LvglDisplayAdapter.cpp>")

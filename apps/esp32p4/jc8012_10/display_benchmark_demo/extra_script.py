from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
# PlatformIO's linker preprocessing does not track sdkconfig.h automatically:
# after a chip revision or memory layout change it would link with the old one.
env.Depends("$BUILD_DIR/memory.ld", "$BUILD_DIR/config/sdkconfig.h")
apps_dir = project_dir.parents[2]
brick = project_dir.parents[3] / "libs" / "brick"
env.Append(CPPPATH=[
    str(brick / "libs" / "interfaces" / "include"),
    str(brick / "libs" / "core" / "include"),
    str(brick / "platforms" / "esp32" / "include"),
    str(brick / "libs" / "lvgl" / "include"),
    str(brick / "boards" / "include"),
    str(apps_dir / "shared" / "benchmark"),
])
env.Append(CPPPATH=[str(project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl" / "src")])

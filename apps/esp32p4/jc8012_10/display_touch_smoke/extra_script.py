from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
# PlatformIO's linker preprocessing does not track sdkconfig.h automatically:
# after a chip revision or memory layout change it would link with the old one.
env.Depends("$BUILD_DIR/memory.ld", "$BUILD_DIR/config/sdkconfig.h")
repo = project_dir.parents[3]
brick = repo / "libs" / "brick"

env.Append(CPPPATH=[
    str(brick / "libs" / "interfaces" / "include"),
    str(brick / "libs" / "core" / "include"),
    str(brick / "platforms" / "esp32" / "include"),
    str(repo / "libs" / "brick" / "boards" / "include"),
])

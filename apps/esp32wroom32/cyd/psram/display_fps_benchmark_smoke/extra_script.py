Import("env")

from pathlib import Path
import subprocess

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[4] / "libs" / "brick"
env.Append(CPPPATH=[
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "libs" / "core" / "include"),
    str(brick_root / "platforms" / "esp32" / "include"),
])

brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(
    str(brick_build_dir / "core"),
    str(brick_root / "libs" / "core" / "src"),
    "+<AssetStreamer.cpp> +<TouchMapper.cpp>",
)
env.BuildSources(str(brick_build_dir / "platform"), str(brick_root / "platforms" / "esp32" / "src"), "+<Ili9341SpiDisplay.cpp>")
env.BuildSources(str(brick_build_dir / "touch"), str(brick_root / "platforms" / "esp32" / "src"), "+<Xpt2046Touchscreen.cpp>")

try:
    version = subprocess.check_output(
        ["git", "-C", str(project_dir), "rev-parse", "--short", "HEAD"],
        text=True,
    ).strip()
except Exception:
    version = "local"

env.Append(CPPDEFINES=[("BRICK_TEST_VERSION", '\\"' + version + '\\"')])

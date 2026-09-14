Import("env")

from pathlib import Path
import subprocess

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[4] / "libs" / "brick"
boards_root = project_dir.parents[4] / "libs" / "brick-boards"
env.Append(CPPPATH=[
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "libs" / "core" / "include"),
    str(brick_root / "platforms" / "esp32" / "include"),
    str(boards_root / "include"),
])

brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(str(brick_build_dir / "core"), str(brick_root / "libs" / "core" / "src"), "+<TouchMapper.cpp>")
env.BuildSources(str(brick_build_dir / "platform"), str(brick_root / "platforms" / "esp32" / "src"),
                 "+<Ili9341SpiDisplay.cpp>+<Xpt2046Touchscreen.cpp>+<FreeRtosTime.cpp>+<EspIdfLogger.cpp>+<File.cpp>+<SdSpiFileSystem.cpp>+<CydSpi3PinMux.cpp>+<CydTouchscreen.cpp>+<CydSdSpiFileSystem.cpp>")

try:
    version = subprocess.check_output(["git", "-C", str(project_dir), "rev-parse", "--short", "HEAD"], text=True).strip()
except Exception:
    version = "local"

env.Append(CPPDEFINES=[("BRICK_TEST_VERSION", '\\"' + version + '\\"')])

from pathlib import Path
import subprocess
import sys
from SCons.Script import COMMAND_LINE_TARGETS

Import("env")
project_dir = Path(env.subst("$PROJECT_DIR"))
repo = project_dir.parents[3]
brick = repo / "libs" / "brick"
env.Append(CPPPATH=[
    str(brick / "libs" / "interfaces" / "include"),
    str(brick / "libs" / "core" / "include"),
    str(brick / "platforms" / "esp32" / "include"),
    str(repo / "libs" / "brick-boards" / "include"),
    str(project_dir / "generated"),
])
env.Append(CPPPATH=[str(project_dir / ".pio" / "libdeps" / env.subst("$PIOENV") / "lvgl" / "src")])
if "clean" not in COMMAND_LINE_TARGETS:
    subprocess.run([sys.executable, str(project_dir / "generate_assets.py")], check=True)
env.Append(FLASH_EXTRA_IMAGES=[("0x610000", str(project_dir / "generated" / "assets.bin"))])

Import("env")

from pathlib import Path
import subprocess

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[3] / "libs" / "brick"
env.Append(CPPPATH=[
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "libs" / "core" / "include"),
    str(brick_root / "platforms" / "esp32" / "include"),
])

try:
    version = subprocess.check_output(
        ["git", "-C", str(project_dir), "rev-parse", "--short", "HEAD"],
        text=True,
    ).strip()
except Exception:
    version = "local"

env.Append(CPPDEFINES=[("BRICK_TEST_VERSION", '\\"' + version + '\\"')])

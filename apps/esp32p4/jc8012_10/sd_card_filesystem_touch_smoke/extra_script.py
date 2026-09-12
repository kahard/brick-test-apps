from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
repo = project_dir.parents[3]
brick_root = repo / "libs" / "brick"
env.Append(CPPPATH=[
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "libs" / "core" / "include"),
    str(brick_root / "platforms" / "esp32" / "include"),
    str(repo / "libs" / "brick-boards" / "include"),
])

from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
repo = project_dir.parents[3]
brick = repo / "libs" / "brick"

env.Append(CPPPATH=[
    str(brick / "libs" / "interfaces" / "include"),
    str(brick / "libs" / "core" / "include"),
    str(brick / "platforms" / "esp32" / "include"),
    str(repo / "libs" / "brick-boards" / "include"),
])

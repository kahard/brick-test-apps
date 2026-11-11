from pathlib import Path


Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
apps_dir = project_dir.parents[3]
brick_root = project_dir.parents[4] / "libs" / "brick"

env.Append(
    CPPPATH=[
        str(brick_root / "libs" / "interfaces" / "include"),
        str(brick_root / "libs" / "core" / "include"),
        str(brick_root / "platforms" / "esp32" / "include"),
        str(brick_root / "libs" / "lvgl" / "include"),
        str(brick_root / "boards" / "include"),
        str(apps_dir / "shared" / "benchmark"),
    ]
)

Import("env")

from pathlib import Path

project_dir = Path(env.subst("$PROJECT_DIR"))
brick_root = project_dir.parents[3] / "libs" / "brick"

env.Append(CPPPATH=[
    str(brick_root / "libs" / "interfaces" / "include"),
    str(brick_root / "platforms" / "stm32" / "include"),
])

brick_build_dir = Path(env.subst("$BUILD_DIR")) / "brick"
env.BuildSources(
    str(brick_build_dir / "platform"),
    str(brick_root / "platforms" / "stm32" / "src"),
    "+<Ssd1963ParallelDisplay.cpp> +<Ft5x06Touchscreen.cpp>",
)

"""Regenerate this project's single asset bundle and typed descriptor table."""
from pathlib import Path
import subprocess
import sys

PROJECT = Path(__file__).resolve().parent
TOOLS = PROJECT.parents[3] / "libs" / "brick" / "tools" / "assets"
WIDTH, HEIGHT = 1024, 600

def run(tool, *arguments):
    subprocess.run([sys.executable, str(TOOLS / tool), *map(str, arguments)], check=True)

def main():
    output = PROJECT / "generated"
    output.mkdir(exist_ok=True)
    entries = []
    for name in ("joy_tears", "sweat_smile"):
        binary = output / (name + ".bin")
        run("generate_image_asset.py", "--input", PROJECT / "assets" / (name + ".png"),
            "--symbol", name, "--format", "rgb565", "--resize", f"{WIDTH}x{HEIGHT}",
            "--binary", binary)
        entries += ["--asset", f"{name}={binary},{WIDTH}x{HEIGHT},rgb565"]
    for name, color in (("red_background", "FF0000"), ("blue_background", "0000FF")):
        binary = output / (name + ".bin")
        run("generate_solid_rgb565.py", "--width", WIDTH, "--height", HEIGHT,
            "--color", color, "--output", binary)
        entries += ["--asset", f"{name}={binary},{WIDTH}x{HEIGHT},rgb565"]
    run("bundle_assets.py", "--output", output / "assets.bin",
        "--header", output / "generated_assets.h", "--manifest", output / "assets.tsv", *entries)

if __name__ == "__main__":
    main()

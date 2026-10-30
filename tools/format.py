#!/usr/bin/env python3
"""Format the application sources with the pinned clang-format.

Usage: python tools/format.py [format|check]
"""

from __future__ import annotations

import os
import sys

import devenv

SOURCE_SUFFIXES = {".c", ".cc", ".cpp", ".cxx", ".h"}


def sources():
    for directory, subdirs, files in os.walk(devenv.APPS_DIR):
        subdirs[:] = sorted(d for d in subdirs if d not in devenv.SKIPPED_DIRS)
        for name in sorted(files):
            if os.path.splitext(name)[1] in SOURCE_SUFFIXES and not name.startswith(("generated_assets", "generated_font")):
                yield os.path.join(directory, name)


def main() -> int:
    mode = sys.argv[1] if len(sys.argv) > 1 else "format"
    if mode not in ("format", "check"):
        sys.exit(__doc__)
    clang_format = devenv.require_venv_tool("clang-format")
    flags = ["-i"] if mode == "format" else ["--dry-run", "--Werror"]
    files = list(sources())
    failed = 0
    # Keep command lines short enough for Windows.
    for start in range(0, len(files), 50):
        if devenv.run([clang_format, *flags, *files[start : start + 50]], cwd=devenv.REPO_ROOT) != 0:
            failed += 1
    print(f"{mode}: {len(files)} files, {'OK' if failed == 0 else 'FAILED'}")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())

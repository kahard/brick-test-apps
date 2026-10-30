#!/usr/bin/env python3
"""Build, clean or list every PlatformIO application under apps/.

Examples:
  python tools/apps.py list
  python tools/apps.py build
  python tools/apps.py build --filter esp32p4/jc1060_7
  python tools/apps.py clean
"""

from __future__ import annotations

import argparse
import shutil
import sys
import time

import devenv


def selected_projects(filters: list[str]):
    projects = devenv.find_projects()
    if filters:
        projects = [p for p in projects if any(f in p.relative_to(devenv.APPS_DIR).as_posix() for f in filters)]
    return projects


def target(project, name: str, jobs: int) -> list:
    """Use the project's Makefile (it generates fonts/assets first) when possible."""
    make = shutil.which("make")
    if make and (project / "Makefile").exists():
        return [make, "-C", project, name, f"JOBS={jobs}"]
    command = [devenv.require_venv_tool("platformio"), "run", "-d", project]
    return command + (["-t", "clean"] if name == "clean" else ["-j", str(jobs)])


def build(project, jobs: int, attempts: int) -> bool:
    for attempt in range(1, max(1, attempts) + 1):
        if attempt > 1:
            # First ESP-IDF builds sometimes fail while components download.
            print(f"Retrying build ({attempt}/{attempts})")
        if devenv.run(target(project, "compile", jobs)) == 0:
            return True
    return False


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=("list", "build", "clean"))
    parser.add_argument("--filter", action="append", default=[], help="only projects whose path contains this text")
    parser.add_argument("-j", "--jobs", type=int, default=2, help="parallel compile jobs per project")
    parser.add_argument("--attempts", type=int, default=2, help="build attempts per project")
    args = parser.parse_args()

    projects = selected_projects(args.filter)
    if not projects:
        print("No matching projects under apps/", file=sys.stderr)
        return 1
    if args.command == "list":
        for project in projects:
            print(project.relative_to(devenv.REPO_ROOT).as_posix())
        return 0

    devenv.require_venv_tool("platformio")
    results = []
    for project in projects:
        name = project.relative_to(devenv.REPO_ROOT).as_posix()
        print(f"\n=== {name} ===", flush=True)
        started = time.monotonic()
        if args.command == "clean":
            ok = devenv.run(target(project, "clean", args.jobs)) == 0
        else:
            ok = build(project, args.jobs, args.attempts)
        results.append((name, ok, time.monotonic() - started))

    print(f"\n=== {args.command} summary ===")
    for name, ok, seconds in results:
        print(f"{'OK    ' if ok else 'FAILED'} {seconds:6.0f}s  {name}")
    failed = sum(1 for _, ok, _ in results if not ok)
    print(f"{len(results) - failed}/{len(results)} succeeded")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())

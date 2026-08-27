import re
from pathlib import Path


def enabled(project: Path, macro: str, default: bool = True) -> bool:
    """Read a BRICK feature switch from the project's board_config.h."""
    for candidate in (project / "include" / "board_config.h", project / "src" / "board_config.h"):
        if not candidate.exists():
            continue
        match = re.search(r"^\s*#\s*define\s+" + re.escape(macro) + r"\s+([01])\s*$", candidate.read_text(encoding="utf-8"), re.MULTILINE)
        if match:
            return match.group(1) == "1"
    return default


def source_expression(project: Path, mapping):
    return " ".join("+<%s>" % source for macro, source in mapping if enabled(project, macro))

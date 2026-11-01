import sys
from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
repository = next(path for path in project_dir.parents if (path / "platformio-versions.ini").is_file())
sys.path.insert(0, str(repository / "tools"))
import network_app_build  # noqa: E402

network_app_build.configure_esp8266(env)

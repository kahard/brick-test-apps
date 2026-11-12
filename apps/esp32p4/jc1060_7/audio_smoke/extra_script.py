import sys
from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
repository = next(path for path in project_dir.parents if (path / "platformio-versions.ini").is_file())
sys.path.insert(0, str(repository / "tools"))
import audio_app_build  # noqa: E402

audio_app_build.configure(env, ["MipiDsiDisplay.cpp", "Gt911Touchscreen.cpp", "I2sPcmOutput.cpp", "Es8311Codec.cpp", "LegacyI2cRegisters.cpp"])

"""Build settings shared by the audio test applications (audio_smoke).

An application's extra_script.py calls configure() with the board sources it
needs (display, touch, audio output); this adds the BRICK include paths, the
PCM audio of the framework core, the shared audio test (apps/shared/audio)
and the text lines it draws with (apps/shared/network/TextLines.cpp).
"""

from __future__ import annotations

from pathlib import Path

from network_app_build import _build_sources, _filter

REPOSITORY = Path(__file__).resolve().parents[1]
BRICK = REPOSITORY / "libs" / "brick"
SHARED_AUDIO = REPOSITORY / "apps" / "shared" / "audio"
SHARED_NETWORK = REPOSITORY / "apps" / "shared" / "network"

CORE_SOURCES = ["PcmSource.cpp", "TouchMapper.cpp"]
ESP32_SOURCES = ["EspIdfLogger.cpp", "FreeRtosTime.cpp"]


def configure(env, board_sources: list[str]) -> None:
    """ESP32 family (ESP-IDF); board_sources: display, touch and audio drivers."""
    project = Path(env.subst("$PROJECT_DIR"))
    env.Append(CPPPATH=[
        str(BRICK / "libs" / "interfaces" / "include"),
        str(BRICK / "libs" / "core" / "include"),
        str(BRICK / "platforms" / "esp32" / "include"),
        str(BRICK / "boards" / "include"),
        str(SHARED_AUDIO),
        str(SHARED_NETWORK),
        str(project / "src" / "app"),
    ])
    build = Path(env.subst("$BUILD_DIR")) / "brick"
    _build_sources(env, str(build / "core"), str(BRICK / "libs" / "core" / "src"), _filter(CORE_SOURCES), False)
    _build_sources(env, str(build / "platform"), str(BRICK / "platforms" / "esp32" / "src"),
                   _filter(board_sources + ESP32_SOURCES), False)
    _build_sources(env, str(build / "audio"), str(SHARED_AUDIO), _filter(["AudioTest.cpp"]), False)
    _build_sources(env, str(build / "lines"), str(SHARED_NETWORK), _filter(["TextLines.cpp"]), False)

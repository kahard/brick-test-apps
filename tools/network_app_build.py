"""Build settings shared by the network test applications.

An application's extra_script.py calls configure() (ESP32 family, with the
board sources it needs) or configure_esp8266(); this adds the BRICK include
paths, the network sources of the framework, the shared test in
apps/shared/network and the Wi-Fi credentials header (see
wifi_credentials.py).
"""
from __future__ import annotations

from pathlib import Path

import wifi_credentials

REPOSITORY = Path(__file__).resolve().parents[1]
BRICK = REPOSITORY / "libs" / "brick"
SHARED = REPOSITORY / "apps" / "shared" / "network"

CORE_SOURCES = [
    "HttpProbe.cpp",
    "Ipv4.cpp",
    "NetworkSelector.cpp",
    "WifiManager.cpp",
    "WifiNetworkChoice.cpp",
    "WifiScanList.cpp",
]
ESP32_SOURCES = [
    "EspDnsResolver.cpp",
    "EspIdfLogger.cpp",
    "EspNetworkStack.cpp",
    "EspTcpSocket.cpp",
    "EspWifi.cpp",
    "EspWifiAccessPoint.cpp",
    "EspWifiStation.cpp",
    "FreeRtosTime.cpp",
]
ESP8266_SOURCES = [
    "Esp8266DnsResolver.cpp",
    "Esp8266TcpSocket.cpp",
    "Esp8266Wifi.cpp",
    "Esp8266WifiStation.cpp",
]
SHARED_SOURCES = ["InternetTest.cpp", "InternetTestScreen.cpp", "TextLines.cpp"]


def _filter(names: list[str]) -> str:
    return " ".join(f"+<{name}>" for name in names)


def _configure(env, platform: str, core_sources: list[str], platform_sources: list[str]) -> None:
    project = Path(env.subst("$PROJECT_DIR"))
    ssid = wifi_credentials.write_header(project / "src" / "target" / "generated" / "wifi_credentials.h")
    print(f"network test: Wi-Fi {ssid!r}" if ssid else "network test: no Wi-Fi configured, hotspot only")
    env.Append(CPPPATH=[
        str(BRICK / "libs" / "interfaces" / "include"),
        str(BRICK / "libs" / "core" / "include"),
        str(BRICK / "platforms" / platform / "include"),
        str(BRICK / "boards" / "include"),
        str(SHARED),
        str(project / "src" / "app"),
    ])
    build = Path(env.subst("$BUILD_DIR")) / "brick"
    env.BuildSources(str(build / "core"), str(BRICK / "libs" / "core" / "src"), _filter(core_sources))
    env.BuildSources(str(build / "platform"), str(BRICK / "platforms" / platform / "src"), _filter(platform_sources))
    env.BuildSources(str(build / "shared"), str(SHARED), _filter(SHARED_SOURCES))


def configure(env, board_sources: list[str], ethernet: bool = False) -> None:
    """ESP32 family (ESP-IDF); board_sources: the display and touch drivers."""
    platform_sources = board_sources + ESP32_SOURCES + (["EspEthernet.cpp"] if ethernet else [])
    _configure(env, "esp32", CORE_SOURCES + ["TouchMapper.cpp"], platform_sources)


def configure_esp8266(env) -> None:
    """ESP8266 (Arduino); the board's own sources are in src/target/brick_sources.cpp.

    The application lists ESP8266WiFi in lib_deps so it is linked; its include
    path is added here because the sources built from this script are set up
    before the library finder adds it.
    """
    framework = Path(env.PioPlatform().get_package_dir("framework-arduinoespressif8266"))
    env.Append(CPPPATH=[str(framework / "libraries" / "ESP8266WiFi" / "src")])
    _configure(env, "esp8266", CORE_SOURCES, ESP8266_SOURCES)

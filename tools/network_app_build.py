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
    "CaptiveDns.cpp",
    "HttpProbe.cpp",
    "HttpServer.cpp",
    "Ipv4.cpp",
    "NetworkSelector.cpp",
    "WifiCredentialStore.cpp",
    "WifiManager.cpp",
    "WifiNetworkChoice.cpp",
    "WifiScanList.cpp",
    "WifiSetupPortal.cpp",
]
ESP32_SOURCES = [
    "EspDnsResolver.cpp",
    "EspIdfLogger.cpp",
    "EspNetworkStack.cpp",
    "EspNvsStorage.cpp",
    "EspTcpListener.cpp",
    "EspTcpSocket.cpp",
    "EspUdpSocket.cpp",
    "EspWifi.cpp",
    "EspWifiAccessPoint.cpp",
    "EspWifiStation.cpp",
    "FreeRtosTime.cpp",
]
ESP8266_SOURCES = [
    "Esp8266DnsResolver.cpp",
    "Esp8266EepromStorage.cpp",
    "Esp8266TcpListener.cpp",
    "Esp8266TcpSocket.cpp",
    "Esp8266UdpSocket.cpp",
    "Esp8266Wifi.cpp",
    "Esp8266WifiStation.cpp",
]
SHARED_SOURCES = ["InternetTest.cpp", "InternetTestScreen.cpp", "TextLines.cpp"]


def _filter(names: list[str]) -> str:
    return " ".join(f"+<{name}>" for name in names)


def _build_sources(env, variant_dir: str, src_dir: str, src_filter: str, keep_extension: bool) -> None:
    """env.BuildSources(), or with keep_extension objects named <file>.cpp.o.

    The ESP8266 linker script puts only *.c.o and *.cpp.o code into flash;
    anything else (BuildSources names objects <file>.o) fills the 32 KB of
    IRAM.
    """
    if not keep_extension:
        env.BuildSources(variant_dir, src_dir, src_filter)
        return
    from SCons.Node import FS
    from SCons.Script import DefaultEnvironment

    nodes = env.CollectBuildFiles(variant_dir, src_dir, src_filter)
    DefaultEnvironment().Append(PIOBUILDFILES=[
        env.Object(target=node.get_abspath() + ".o", source=node) if isinstance(node, FS.File) else node
        for node in nodes
    ])


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
    flash_code = platform == "esp8266"
    _build_sources(env, str(build / "core"), str(BRICK / "libs" / "core" / "src"), _filter(core_sources), flash_code)
    _build_sources(env, str(build / "platform"), str(BRICK / "platforms" / platform / "src"),
                   _filter(platform_sources), flash_code)
    _build_sources(env, str(build / "shared"), str(SHARED), _filter(SHARED_SOURCES), flash_code)


def configure(env, board_sources: list[str], ethernet: bool = False) -> None:
    """ESP32 family (ESP-IDF); board_sources: the display and touch drivers."""
    platform_sources = board_sources + ESP32_SOURCES + (["EspEthernet.cpp"] if ethernet else [])
    _configure(env, "esp32", CORE_SOURCES + ["TouchMapper.cpp"], platform_sources)


def configure_esp8266(env) -> None:
    """ESP8266 (Arduino); the board's own sources are in src/target/brick_sources.cpp.

    The application lists ESP8266WiFi and EEPROM in lib_deps so they are
    linked; their include paths are added here because the sources built from
    this script are set up before the library finder adds them.
    """
    framework = Path(env.PioPlatform().get_package_dir("framework-arduinoespressif8266"))
    env.Append(CPPPATH=[str(framework / "libraries" / name) for name in ("ESP8266WiFi/src", "EEPROM")])
    _configure(env, "esp8266", CORE_SOURCES, ESP8266_SOURCES)

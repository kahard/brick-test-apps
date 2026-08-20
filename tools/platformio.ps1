$ErrorActionPreference = "Stop"
$Arguments = $args

# ESP-IDF rejects Git Bash/MSYS2 variables. PlatformIO may otherwise install
# only tool metadata and leave the esptool Python module unavailable.
@(
    "MSYSTEM",
    "MSYSTEM_CARCH",
    "MSYSTEM_CHOST",
    "MSYSTEM_PREFIX",
    "MINGW_CHOST",
    "MINGW_PACKAGE_PREFIX",
    "MINGW_PREFIX",
    "SHELL",
    "EXEPATH"
) | ForEach-Object {
    Remove-Item -Path "Env:$_" -ErrorAction SilentlyContinue
}

$platformio = Get-Command platformio.exe -ErrorAction Stop
& $platformio.Source @Arguments
exit $LASTEXITCODE

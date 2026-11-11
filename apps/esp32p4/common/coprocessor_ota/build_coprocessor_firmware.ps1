# Builds the esp_hosted co-processor firmware for the ESP32-C6 of the Guition
# ESP32-P4 panels, from the esp_hosted component this application uses (run
# `make compile` first: it downloads the component) and with the ESP-IDF that
# PlatformIO installed. The result is .pio\coprocessor\network_adapter.bin.
#
#   powershell -ExecutionPolicy Bypass -File build_coprocessor_firmware.ps1
#
# idf.py needs its Python packages: a virtual environment with them is made
# once in .pio\coprocessor\venv. It refuses to run from MSYS / Git Bash.

$ErrorActionPreference = "Stop"
$App      = $PSScriptRoot
$Packages = Join-Path $env:USERPROFILE ".platformio\packages"
$IdfPath  = Join-Path $Packages "framework-espidf"
$Hosted   = Join-Path $App "managed_components\espressif__esp_hosted"
$Work     = Join-Path $App ".pio\coprocessor"
$Venv     = Join-Path $Work "venv"
$Python   = Join-Path $Venv "Scripts\python.exe"

if (-not (Test-Path (Join-Path $Hosted "slave"))) { throw "Run 'make compile' first: $Hosted\slave is missing." }
if (-not (Test-Path (Join-Path $IdfPath "tools\idf.py"))) { throw "ESP-IDF not found in $IdfPath (build any ESP32 application first)." }

New-Item -ItemType Directory -Force $Work | Out-Null
# The firmware project looks for the shared sources in ..\common.
foreach ($Part in "slave", "common") {
    $Target = Join-Path $Work $Part
    if (Test-Path $Target) { Remove-Item -Recurse -Force $Target }
    Copy-Item -Recurse (Join-Path $Hosted $Part) $Target
}

if (-not (Test-Path $Python)) {
    $BasePython = Join-Path $env:USERPROFILE ".platformio\penv\Scripts\python.exe"
    & $BasePython -m venv $Venv
    if ($LASTEXITCODE -ne 0) { throw "Creating $Venv failed." }
    & $Python -m pip install -q -r (Join-Path $IdfPath "tools\requirements\requirements.core.txt")
    if ($LASTEXITCODE -ne 0) { throw "Installing the ESP-IDF Python packages failed." }
}

# idf.py checks the packages against a constraints file of the IDF tools
# directory; the packages above are the ones it needs, so an empty file will do.
$Tools = Join-Path $Work "idf-tools"
New-Item -ItemType Directory -Force $Tools | Out-Null
$Constraints = Join-Path $Tools "espidf.constraints.v5.5.txt"
if (-not (Test-Path $Constraints)) { New-Item -ItemType File $Constraints | Out-Null }

$env:IDF_PATH            = $IdfPath
$env:IDF_TOOLS_PATH      = $Tools
$env:IDF_PYTHON_ENV_PATH = $Venv
$env:PYTHONIOENCODING    = "utf-8"
Remove-Item Env:MSYSTEM -ErrorAction SilentlyContinue
$env:PATH = "$Packages\toolchain-riscv32-esp\bin;$Packages\tool-cmake\bin;$Packages\tool-ninja;$Venv\Scripts;" + $env:PATH

Push-Location (Join-Path $Work "slave")
try {
    & $Python (Join-Path $IdfPath "tools\idf.py") -B build set-target esp32c6
    if ($LASTEXITCODE -ne 0) { throw "idf.py set-target failed." }
    & $Python (Join-Path $IdfPath "tools\idf.py") -B build build
    if ($LASTEXITCODE -ne 0) { throw "idf.py build failed." }
}
finally {
    Pop-Location
}
$Image = Join-Path $Work "network_adapter.bin"
Copy-Item (Join-Path $Work "slave\build\network_adapter.bin") $Image -Force
Write-Host "Co-processor firmware: $Image"

param(
    [string]$Root = (Resolve-Path (Join-Path $PSScriptRoot ".."))
)
$ErrorActionPreference = "Stop"

$vscode = Join-Path $Root ".vscode"
New-Item -ItemType Directory -Force -Path $vscode | Out-Null

$properties = [ordered]@{
    configurations = @([ordered]@{
        name = "BRICK test apps"
        compilerPath = '${env:USERPROFILE}/.platformio/packages/toolchain-xtensa-esp-elf/bin/xtensa-esp32s3-elf-g++.exe'
        cStandard = "c17"
        cppStandard = "c++17"
        includePath = @(
            '${workspaceFolder}/libs/brick/libs/interfaces/include'
            '${workspaceFolder}/libs/brick/libs/core/include'
            '${workspaceFolder}/libs/brick/platforms/esp32/include'
            '${workspaceFolder}/libs/brick/platforms/esp8266/include'
            '${workspaceFolder}/libs/brick/platforms/stm32/include'
            '${workspaceFolder}/libs/brick-boards/include'
            '${workspaceFolder}/apps/**/include'
            '${workspaceFolder}/apps/**/src'
            '${workspaceFolder}/apps/**/.pio/libdeps/**/include'
            '${workspaceFolder}/.pio/libdeps/**/include'
            '${env:USERPROFILE}/.platformio/packages/framework-espidf/components/**/include'
            '${env:USERPROFILE}/.platformio/packages/framework-espidf/components/**/include/**'
        )
        defines = @("ESP_PLATFORM", "ARDUINO_ARCH_ESP32")
        browse = [ordered]@{
            path = @('${workspaceFolder}/libs/brick', '${workspaceFolder}/libs/brick-boards', '${workspaceFolder}/apps')
            limitSymbolsToIncludedHeaders = $true
        }
    })
    version = 4
}
$properties | ConvertTo-Json -Depth 8 | Set-Content (Join-Path $vscode "c_cpp_properties.json") -Encoding utf8

$settings = [ordered]@{
    "cmake.ignoreCMakeListsMissing" = $true
    "C_Cpp.default.configurationProvider" = "platformio.platformio-ide"
}
$settings | ConvertTo-Json | Set-Content (Join-Path $vscode "settings.json") -Encoding utf8
Write-Output "VS Code IntelliSense configuration written to $vscode"

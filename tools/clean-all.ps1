[CmdletBinding()]
param(
    [string]$MakeCommand = "make"
)

$ErrorActionPreference = "Continue"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot ".."))
$appRoot = Join-Path $repoRoot "apps"

$makefiles = Get-ChildItem -LiteralPath $appRoot -Recurse -Filter "Makefile" -File |
    Where-Object { $_.FullName -notmatch "\\(\.git|\.pio)\\" } |
    Sort-Object FullName

if ($makefiles.Count -eq 0) {
    Write-Error "Nie znaleziono Makefile w katalogu apps."
    exit 1
}

$results = @()

foreach ($makefile in $makefiles) {
    $appDirectory = $makefile.DirectoryName
    $appName = $appDirectory.Substring($repoRoot.Path.Length).TrimStart('\')
    $cleanOk = $false

    Write-Host ""
    Write-Host "=== $appName ===" -ForegroundColor Cyan

    Push-Location $appDirectory
    try {
        Write-Host "> $MakeCommand clean" -ForegroundColor DarkGray
        & $MakeCommand clean
        $cleanOk = ($LASTEXITCODE -eq 0)
        if ($cleanOk) {
            Write-Host "clean: OK" -ForegroundColor Green
        } else {
            Write-Host "clean: FAILED (exit $LASTEXITCODE)" -ForegroundColor Red
        }
    } catch {
        Write-Host "wyjątek: $($_.Exception.Message)" -ForegroundColor Red
    } finally {
        Pop-Location
    }

    $results += [PSCustomObject]@{
        Application = $appName
        Clean = $cleanOk
    }
}

Write-Host ""
Write-Host "=== Podsumowanie clean-all ===" -ForegroundColor Cyan
$results | Format-Table -AutoSize

$failed = @($results | Where-Object { -not $_.Clean })
if ($failed.Count -eq 0) {
    Write-Host "Wszystkie aplikacje zostały wyczyszczone poprawnie." -ForegroundColor Green
    exit 0
}

Write-Host "Niepowodzenia: $($failed.Count) z $($results.Count) aplikacji." -ForegroundColor Red
exit 1

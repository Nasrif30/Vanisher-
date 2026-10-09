# Builds local source only. Does not connect to or reset a printer.
$ErrorActionPreference = 'Stop'
Write-Host 'VANISHER | Created by ALNASRIF HALIDDIN'
Write-Host 'Building the Windows x64 app. No printer will be accessed.'
Write-Host 'The first build downloads a pinned JSON source header and verifies its SHA256 hash.'
try {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere)) {
        throw 'Install Visual Studio 2022 Build Tools with Desktop development with C++. See README.md.'
    }
    $installation = & $vswhere -latest -products '*' -version '[17.0,18.0)' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($LASTEXITCODE -ne 0 -or -not $installation) {
        throw 'Visual Studio 2022 C++ tools were not found. Install Desktop development with C++, the Windows SDK, and C++ CMake tools. See README.md.'
    }
    $cmake = Join-Path $installation 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
    if (-not (Test-Path -LiteralPath $cmake)) {
        $command = Get-Command cmake.exe -ErrorAction SilentlyContinue
        if (-not $command) { throw 'CMake was not found. Add C++ CMake tools for Windows in Visual Studio Installer.' }
        $cmake = $command.Source
    }
    # Ask Microsoft build tools to display English messages.
    $env:VSLANG = '1033'
    $buildFolder = Join-Path $PSScriptRoot 'build'
    & $cmake -S $PSScriptRoot -B $buildFolder -G 'Visual Studio 17 2022' -A x64 '-DEWR_BUILD_TESTS=OFF' '-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded'
    if ($LASTEXITCODE -ne 0) { throw 'Configuration failed. Read the messages above; check internet access, the C++ tools, and the Windows SDK installation.' }
    & $cmake --build $buildFolder --config Release
    if ($LASTEXITCODE -ne 0) { throw 'Compilation failed. Read the messages above. No printer was accessed.' }
    Copy-Item -LiteralPath (Join-Path $PSScriptRoot 'Release\VANISHER.exe') -Destination (Join-Path $PSScriptRoot 'VANISHER.exe') -Force
    Write-Host ''
    Write-Host 'BUILD COMPLETE: VANISHER.exe is ready in this folder.'
    Write-Host 'Double-click Validate L4260.cmd for a local check, or Open VANISHER.cmd to open the menu.'
    Write-Host 'No printer reset was performed.'
    exit 0
} catch {
    Write-Host ''
    Write-Host ('BUILD FAILED: ' + $_.Exception.Message)
    Write-Host 'Read README.md for the one-time setup instructions.'
    exit 1
}

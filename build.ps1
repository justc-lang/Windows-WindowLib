param(
    [string]$Configuration = "Release",
    [string]$Platform = "x64"
)

if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}

Push-Location build

cmake .. -DCMAKE_GENERATOR_PLATFORM=$Platform -DCMAKE_SYSTEM_VERSION=10.0.19041.0
cmake --build . --config $Configuration --parallel

Pop-Location

Write-Host "Build completed successfully!"
Write-Host "Output: build/src/WindowLib/$Configuration/"
Write-Host "Test App: build/src/TestApp/$Configuration/TestApp.exe"

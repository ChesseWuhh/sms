@echo off
echo ========================================
echo    Student Management System
echo    Build Script for Windows
echo ========================================
echo.

REM Check prerequisites
echo [INFO] Checking prerequisites...

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake is not installed.
    echo Please install CMake from: https://cmake.org/download/
    pause
    exit /b 1
)

where ninja >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Ninja is not installed.
    echo Please download Ninja from: https://github.com/ninja-build/ninja/releases
    echo Add ninja.exe to your PATH environment variable.
    pause
    exit /b 1
)

echo [INFO] CMake: $(where cmake)
echo [INFO] Ninja: $(where ninja)

REM Clean previous build
echo.
echo [INFO] Cleaning previous build...
if exist build rmdir /s /q build
mkdir build
cd build

REM Configure with Ninja
echo.
echo [INFO] Configuring project with Ninja generator...
echo [INFO] Build type: Release

cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Configuration failed!
    echo Please check the CMake output above for errors.
    pause
    exit /b 1
)

REM Build project
echo.
echo [INFO] Building project...

REM Get number of CPU cores
for /f "tokens=2 delims==" %%I in ('wmic cpu get NumberOfCores /value') do (
    for /f "delims=" %%J in ("%%I") do set CORES=%%J
)

echo [INFO] Using %CORES% parallel jobs
ninja -j%CORES%

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed!
    echo Please check the compilation errors above.
    pause
    exit /b 1
)

REM Success message
echo.
echo [SUCCESS] Build completed successfully!
echo.


echo.
pause
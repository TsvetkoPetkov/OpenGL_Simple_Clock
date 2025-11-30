@echo off
setlocal

REM Build the project with CMake (no vcpkg). CMake will fetch GLFW via FetchContent.
REM This script does not install CMake or Visual Studio.
REM Usage:
REM   scripts\build.bat [Debug|Release]

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set ROOT_DIR=%~dp0\..
set BUILD_DIR=%ROOT_DIR%\build

echo === ClockSimulation Build (%BUILD_TYPE%) ===

where cmake >nul 2>&1 || (echo [ERROR] CMake not found in PATH. Please install CMake 3.20+ and add to PATH. & exit /b 1)

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo [INFO] Configuring CMake...
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 ( echo [ERROR] Configuration failed. & exit /b 1 )

echo [INFO] Building...
cmake --build "%BUILD_DIR%" --config %BUILD_TYPE%
if errorlevel 1 ( echo [ERROR] Build failed. & exit /b 1 )

echo.
echo [OK] Build completed. Artifacts in: %BUILD_DIR%
exit /b 0

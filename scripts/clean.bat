@echo off
setlocal

REM Remove the CMake build directory.

set ROOT_DIR=%~dp0\..
set BUILD_DIR=%ROOT_DIR%\build

if exist "%BUILD_DIR%" (
  echo Deleting %BUILD_DIR% ...
  rmdir /S /Q "%BUILD_DIR%"
) else (
  echo Build directory not found: %BUILD_DIR%
)

exit /b 0


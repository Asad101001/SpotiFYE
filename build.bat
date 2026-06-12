@echo off
echo =======================================
echo Building Aura Music (C++ / Raylib)...
echo =======================================

if not exist build mkdir build

:: Compile main.cpp and link with raylib statically
g++ src\main.cpp -o build\AuraMusic.exe -I lib\include -L lib\lib -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed! Check your code for syntax errors.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Build completed successfully!
echo.
echo Starting Application...
build\AuraMusic.exe

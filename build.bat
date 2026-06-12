@echo off
echo =====================================
echo   SpotiFYE - DSA Music Player Build
echo =====================================

if not exist build mkdir build

g++ src\main.cpp -o build\SpotiFYE.exe -I lib\include -L lib\lib -lraylib -lopengl32 -lgdi32 -lwinmm

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed! Check your code for syntax errors.
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Build complete!
echo.
echo Launching SpotiFYE...
build\SpotiFYE.exe

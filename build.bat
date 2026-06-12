@echo off
cls

echo [1/3] Compiling application icon...
windres assets\resource.rc -O coff -o assets\resource.res

echo [2/3] Compiling SpotiFYE...
g++ src\main.cpp assets\resource.res -o build\SpotiFYE.exe -I lib\include -L lib\lib -lraylib -lopengl32 -lgdi32 -lwinmm -static-libgcc -static-libstdc++

if %errorlevel% neq 0 (
    echo.
    echo [!] Compilation failed!
    pause
    exit /b %errorlevel%
)

echo [3/3] Compilation successful!
echo.
echo Launching SpotiFYE...
start "" "build\SpotiFYE.exe"

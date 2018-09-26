@echo off
cd /d "%~dp0"
if exist build rd /s /q build
md build
cd build
lupdate -no-obsolete "%~dp0src\dd.pro"
lrelease -nounfinished -removeidentical "%~dp0src\dd.pro"
qmake "%~dp0src\dd.pro" -spec win32-msvc "CONFIG+=release"
jom qmake_all
jom && jom install
if exist "%~dp0bin" (
    cd "%~dp0bin"
    windeployqt --plugindir "%~dp0bin\plugins" --release --force --no-translations --compiler-runtime --angle --no-opengl-sw dd.exe
)
if exist "%~dp0bin64" (
    cd "%~dp0bin64"
    windeployqt --plugindir "%~dp0bin64\plugins" --release --force --no-translations --compiler-runtime --angle --no-opengl-sw dd64.exe
)
cd "%~dp0"
rd /s /q build
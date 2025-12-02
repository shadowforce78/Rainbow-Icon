cls
@echo off
echo "Stopping Geometry Dash...."
@echo off
taskkill /f /im GeometryDash.exe
echo "Geometry Dash stopped!"
echo "Starting build...."
geode build -- -G "Visual Studio 17 2022" -D CMAKE_TLS_VERIFY=OFF
echo "Windows build complete!"
echo "Do you want to start Geometry Dash? (y/n)"
set /p choice=
if %choice% == y goto start
if %choice% == n goto end
:start
echo "Starting Geometry Dash...."
start steam://rungameid/322170
goto end
:end
echo "Press any key to exit..."
pause >nul

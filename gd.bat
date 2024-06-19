cls
@echo off
echo "Stopping Geometry Dash...."
@echo off
taskkill /f /im GeometryDash.exe
echo "Geometry Dash stopped!"
echo "Starting Geometry Dash...."
start steam://rungameid/322170
goto end
:end
echo "Press any key to exit..."
pause >nul
cls
@echo off
@REM Ask for the .geode file link
echo "Enter the .geode file link:"
set /p geode=
@REM Execute the index submit command
geode index submit create
@REM Past the value of the .geode file link
%geode%
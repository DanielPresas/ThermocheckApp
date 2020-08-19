@echo off

echo Deleting files...
echo.

for /d /r . %%d in (.vs) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (Thermocheck\Build) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (Externals\Build) do @if exist "%%d" rd /s /q "%%d"

for /d /r %%d in (Thermocheck) del /s /q *.vcxproj
for /d /r %%d in (Thermocheck) del /s /q *.vcxproj.filters
for /d /r %%d in (Thermocheck) del /s /q *.vcxproj.user
del /q Externals\*.vcxproj
del /q Externals\*.vcxproj.filters
del /q Externals\*.vcxproj.user
del /q ThermocheckApp.sln

echo.
echo Complete.
echo.

pause
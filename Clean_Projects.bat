@echo off

echo Deleting files...
echo.

for /d /r . %%d in (.vs) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (VideoDecoder\Build) do @if exist "%%d" rd /s /q "%%d"
for /d /r . %%d in (Externals\Build) do @if exist "%%d" rd /s /q "%%d"

del /s /q *.vcxproj
del /s /q *.vcxproj.filters
del /s /q *.vcxproj.user
del /s /q *.sln

echo.
echo Complete.
echo.

pause
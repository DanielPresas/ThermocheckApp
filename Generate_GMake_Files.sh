#!/usr/bin/env bash

echo Generating Thermocheck app files...

CURRENT_LOC=$(pwd)
./Externals/premake/premake5 gmake2 --file="$CURRENT_LOC/premake5.lua"

echo Complete.
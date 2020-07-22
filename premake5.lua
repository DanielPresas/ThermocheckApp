print("Starting premake build...")

print("Building solution Thermocheck...\n")
workspace "ThermocheckApp"
    defines "_CRT_SECURE_NO_WARNINGS"
    architecture "x64"

    warnings "extra"
    disablewarnings {
        "4100",
        "4201",
        "4505"
    }

    configurations {
        "Debug",
        "Release",
        "Distribution"
    }

    startproject "Thermocheck"

include "Externals"
include "Thermocheck"
print("Starting premake build...")
print("")

print("Building solution VideoDecoder...")
workspace "VideoDecoder"
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

include "Externals"
include "VideoDecoder"
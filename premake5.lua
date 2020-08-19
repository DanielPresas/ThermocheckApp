workspace "ThermocheckApp"
    defines "_CRT_SECURE_NO_WARNINGS"

    warnings "extra"
        
    configurations {
        "Debug",
        "Release",
        "Distribution"
    }

    startproject "Thermocheck"
    
    filter "system:windows"
        architecture "x86_64"
        disablewarnings {
            "4100",
            "4201",
            "4505",
            "4996",
        }
    
    filter "system:linux"
        architecture "ARM"
        disablewarnings {
            "unused-function",
            "unused-parameter"
        }

include "Externals"
include "Thermocheck"

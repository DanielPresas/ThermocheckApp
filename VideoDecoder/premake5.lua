print("Building project \"VideoDecoder\"...")
print("")
project "VideoDecoder"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    staticruntime "on"

    targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
    objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"

    pchheader "vdpch.h"
	pchsource "src/vdpch.cpp"

    defines { "GLFW_INCLUDE_NONE" }

    flags { "FatalWarnings" }

    includedirs {
        "%{wks.location}/Externals/ffmpeg/include",
        "%{wks.location}/Externals/glad/include",
        "%{wks.location}/Externals/glfw3/include",
        -- "%{wks.location}/Externals/glm/include",
        -- "%{wks.location}/Externals/imgui/include",
        "%{wks.location}/Externals/spdlog/include",
        -- "%{wks.location}/Externals/stb/include",
        
        "include"
    }

    links {
        "Externals",
        "opengl32"
    }

    files {
        "include/**.h",
        "include/**.hpp",
        "include/**.inl",
        "src/**.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "PLATFORM_WINDOWS"
        }

    filter "system:linux"
        systemversion "latest"
        defines {
            "PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines {
            "_DEBUG=1",
            "_RELEASE=0",
            "_DIST=0",
            "_ENABLE_ASSERTS=1"
        }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines {
            "_DEBUG=0",
            "_RELEASE=1",
            "_DIST=0",
            "_ENABLE_ASSERTS=0"
        }
        runtime "Release"
        optimize "on"
        
    filter "configurations:Distribution"
        defines {
            "_DEBUG=0",
            "_RELEASE=0",
            "_DIST=1",
            "_ENABLE_ASSERTS=0"
        }
        runtime "Release"
        optimize "on"
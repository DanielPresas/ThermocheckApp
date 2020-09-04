project "Externals"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    staticruntime "on"

    targetdir "Build/bin/%{cfg.architecture}/%{cfg.buildcfg}"
    objdir "Build/obj/%{cfg.architecture}/%{cfg.buildcfg}"

    warnings "default"

	includedirs {
        "glad/include",
        "glfw3/include",
        "imgui/include/imgui",
        "libuvc/include/libuvc",
        "libuvc/build/include",
        "opencv/install/opencv/include",
        "stb/include/stb"
    }

    files {
        "glad/include/KHR/khrplatform.h",
        "glad/include/glad/glad.h",
        "glad/src/glad.c",
        
        "glfw3/include/glfw/glfw3.h",
        "glfw3/include/glfw/glfw3native.h",
        "glfw3/src/glfw_config.h",
        "glfw3/src/internal.h",
        "glfw3/src/mappings.h",
        "glfw3/src/context.c",
        "glfw3/src/init.c",
        "glfw3/src/input.c",
        "glfw3/src/monitor.c",
        "glfw3/src/vulkan.c",
        "glfw3/src/window.c",

        "imgui/include/imgui/imconfig.h",
        "imgui/include/imgui/imgui.h",
        "imgui/include/imgui/imgui_internal.h",
        "imgui/include/imgui/imstb_rectpack.h",
        "imgui/include/imgui/imstb_textedit.h",
        "imgui/include/imgui/imstb_truetype.h",
        "imgui/include/imgui/imgui.cpp",
        "imgui/include/imgui/imgui_demo.cpp",
        "imgui/include/imgui/imgui_draw.cpp",
        "imgui/include/imgui/imgui_widgets.cpp",

        "libuvc/src/ctrl.c",
        "libuvc/src/ctrl-gen.c",
        "libuvc/src/device.c",
        "libuvc/src/diag.c",
        "libuvc/src/frame.c",
        "libuvc/src/init.c",
        "libuvc/src/misc.c",
        "libuvc/src/stream.c",
        
        "stb/include/stb/stb_image.h",
        "stb/include/stb/stb_image_write.h",
        "stb/include/stb/stb_impl.h",
        "stb/include/stb/stb_include.h",
        "stb/include/stb/stb_perlin.h",
        "stb/include/stb/stb_rect_pack.h",
        "stb/include/stb/stb_truetype.h",
        "stb/src/stb_impl.cpp"
    }

    filter "system:linux"
        files {
            "glfw3/src/egl_context.h",
            "glfw3/src/glx_context.h",
            "glfw3/src/linux_joystick.h",
            "glfw3/src/osmesa_context.h",
            "glfw3/src/posix_thread.h",
            "glfw3/src/posix_time.h",
            "glfw3/src/xkb_unicode.h",
            "glfw3/src/x11_platform.h",
            "glfw3/src/egl_context.c",
            "glfw3/src/glx_context.c",
            "glfw3/src/linux_joystick.c",
            "glfw3/src/osmesa_context.c",
            "glfw3/src/posix_time.c",
            "glfw3/src/posix_thread.c",
            "glfw3/src/xkb_unicode.c",
            "glfw3/src/x11_init.c",
            "glfw3/src/x11_monitor.c",
            "glfw3/src/x11_window.c",
        }

        defines {
            "_GLFW_X11"
        }
		
    filter "system:windows"
        systemversion "latest"

        disablewarnings {
            "4005",
            "4101",
            "4244",
        }
        
        includedirs {
		    "libusb/libusb",
       	    "pthread/include",
			"libusb/msvc"
        }

        files {
            "glfw3/src/win32_init.c",
            "glfw3/src/win32_joystick.c",
            "glfw3/src/win32_monitor.c",
            "glfw3/src/win32_time.c",
            "glfw3/src/win32_thread.c",
            "glfw3/src/win32_window.c",
            "glfw3/src/wgl_context.c",
            "glfw3/src/egl_context.c",
            "glfw3/src/osmesa_context.c",
			
			"libusb/libusb/hotplug.h",
			"libusb/libusb/libusb.h",
			"libusb/libusb/libusbi.h",
			"libusb/libusb/version.h",
			"libusb/libusb/version_nano.h",
			"libusb/libusb/core.c",
			"libusb/libusb/descriptor.c",
			"libusb/libusb/hotplug.c",
			"libusb/libusb/io.c",
			"libusb/libusb/strerror.c",
			"libusb/libusb/sync.c",

            "libusb/msvc/config.h",
            "libusb/libusb/os/poll_windows.h",
            "libusb/libusb/os/threads_windows.h",
            "libusb/libusb/os/windows_common.h",
            "libusb/libusb/os/windows_winusb.h",
            "libusb/libusb/os/windows_nt_common.h",
            "libusb/libusb/os/poll_windows.c",
            "libusb/libusb/os/threads_windows.c",
            "libusb/libusb/os/windows_nt_common.c",
            "libusb/libusb/os/windows_winusb.c",
        }

        defines {
            "_CRT_NONSTDC_NO_WARNINGS",
            "_CRT_SECURE_NO_WARNINGS",
            "_MT",
            "_GLFW_WIN32",
            "HAVE_PTW32_CONFIG_H",
            "WIN32"
        }
        
        libdirs {
            -- "libusb/x64/Release/dll",
            "pthread/lib/x64",
        }
    
        links {
            -- "libusb-1.0"
            "pthreadVC2"
        }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"

    filter "configurations:Distribution"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"


# Features

## Current build

- [ImGui](https://https://github.com/ocornut/imgui) window with video capture and device selection options
- Ability to select between face detection mode and pre-defined region of interest (ROI)
- Console log and metrics window, and about section with a small description

Class/File | Purpose
-|-
Application | Main application class, entry point for the program, handles window creation, graphics context, initialization and shutdown, as well as threaded events
CVInterface | Handles all video capture streaming and rendering, as well as any ROI detection and calculation for the video itself
CaptureDevice | Abstraction for video device information
GraphicsContext | Handles OpenGL context creation and debugging
ImGuiBuild | Allows ImGui to access the native GLFW and Glad setup for OpenGL rendering
ImGuiConsole | ImGui integration of the messages in the standard log flushed to the terminal
ImGuiLayer | GUI Layer, used to contain information about windows and behaviour about controls that do not directly have to do with the video capture interface (e.g. ImGuiConsole, metrics, about)
Logging | Abstraction for [spdlog](https://github.com/gabime/spdlog), allows for very fast and convenient logging anywhere
Random | Utility library to generate random numbers of any type and size
Timer | Utility library to profile parts of the program (e.g. the face detection algorithm)
Window | Handles GLFW windowing system for OpenGL rendering

## Future features

- Device list with manufacturer, product, serial number, and vendor and product IDs (already implemented in `uvc-device` branch)
- Get temperature values directly from FLIR Lepton using `libuvc` and the Lepton SDK (code based on [GetThermal](https://github.com/groupgets/GetThermal) by GroupGets)
- Apply temperature thresholds and alerts that occur when high temperature conditions are met
- Let user define non-face-detected ROI via mouse dragging, property definition, etc.
- Allow for different colorization and visualization options of the thermal video stream (optional)

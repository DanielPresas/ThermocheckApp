# Video Decoder

A simple project that uses OpenCV to get thermal image data.

## Building for Windows

### **Requirements**
- [Git Bash](https://gitforwindows.org/)
- [CMake](https://cmake.org/) - Make sure to add to **PATH** variable wehen prompted!
- [Python 2](https://www.python.org/downloads/release/python-2718/) and [Python 3](https://www.python.org/downloads/release/python-378/)
- [Microsoft Visual Studio 2019 Community](https://visualstudio.microsoft.com/downloads/)

### To build (Windows):
1. Navigate to `Externals/opencv` and open a Git Bash command prompt. Input the following:
    ```sh
    ./installOCV.sh
    ```
    You can have a coffee or two while this builds.
2. Navigate to the root of the project and run the `Generate_VS2019_Project.bat`. All VS2019 project files will be generated.
    - If you use VS2017, there is a project generator for it as well.
3. Open the generated `.sln` file, choose your build configuration (Debug, Release or Distribution) and build using `Ctrl` + `Shift`+ `B`, or by navigating to `Build > Build Solution`.

To test in the VS2019 environment, simply click "Local Windows Debugger at the top, or press `F5` (`Ctrl` + `F5` if you want to start without the debugger).
Current build should show: Starry night by Van Gogh in a window named "Thermocheck App"

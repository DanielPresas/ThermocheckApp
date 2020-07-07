# Video Decoder
Simple video decoder using OpenCV

## To build (Windows)
1. Navigate to `Externals/opencv` and run the `installOCV.sh` file. You should be able to run this script from Git Bash, which can be downloaded [here](https://gitforwindows.org/). [MSYS2](https://www.msys2.org/) is a good alternative as well. [CMake](https://cmake.org/) is also a requirement; make sure to add it to the **PATH** variable during installation.
2. Run the `Generate_VS20XX_Project.bat` file according to the Visual Studio version you use. All VS project files will be generated.
3. Open the `.sln` file, choose your build configuration (Debug, Release or Distribution) and build using `Ctrl` + `Shift`+ `B`. An executable will appear in `VideoDecoder/Build/bin/x86_64/{config}/`, which you can run directly.
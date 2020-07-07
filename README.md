# Video Decoder

A simple project that uses OpenCV to get thermal image data.

## Building for Windows

### **Requirements**
- [Git Bash](https://gitforwindows.org/)
- [CMake](https://cmake.org/) - Make sure to add to **PATH** variable wehen prompted!
- [Python 2](https://www.python.org/downloads/release/python-2718/) and [Python 3](https://www.python.org/downloads/release/python-378/)
- [Microsoft Visual Studio 2019 Community](https://visualstudio.microsoft.com/downloads/)

## To build (Windows):
1. Navigate to `Externals/opencv` and open a Git Bash command prompt. Input the following:
    ```sh
    ./installOCV.sh
    ```
    You can have a coffee or two while this builds.
2. Navigate to the root of the project and run the `Generate_VS2019_Project.bat`. All VS2019 project files will be generated.
    - If you use VS2017, there is a project generator for it as well.
3. Open the generated `.sln` file, choose your build configuration (Debug, Release or Distribution) and build using `Ctrl` + `Shift`+ `B`, or by navigating to `Build > Build Solution`.

## Testing:
1. Copy the `VideoDecoder/Assets` folder to `Build\bin\{arch}\{cfg}` where the built executable is located. (Replace {arch} and {cfg} with the configuration you built with--by default this is `Build\bin\x86_64\Debug`.)
2. Open a command prompt and run 
    ```cmd
    {Repo_dir}\VideoDecoder\Build\bin\{arch}\{cfg}\VideoDecoder.exe Assets/opencv-logo.png
    ```
    replacing {Repo_dir} with the directory where you cloned this repo, and replacing {arch} and {cfg} with the configuration you built with (again, default is `Build\bin\x86_64\Debug`)
3. You should see a window open up with the OpenCV logo rendered in it!

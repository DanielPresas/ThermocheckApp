# Thermocheck App

A simple project that uses OpenCV to get thermal image data and performs facial recognition.

## **Building for Windows 10**

### *Requirements:*

- [Git Bash](https://gitforwindows.org/)
- [CMake](https://cmake.org/) - Make sure to add to **PATH** variable when prompted!
- [Python 2](https://www.python.org/downloads/release/python-2718/) and [Python 3](https://www.python.org/downloads/release/python-378/)
- [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

### *Building for Windows:*

1. Navigate to `Externals/opencv` and open a Git Bash window. Input the following:

    ```sh
    ./install-opencv-windows.sh
    ```

    You can have a coffee or two while this builds.

2. Navigate to the root of the project and run the `Generate_VS2019_Project.bat` file. All VS2019 project files will be generated.
    - If you use VS2017, there is a project generator for it as well.
3. Open the generated `.sln` file, choose your build configuration (Debug, Release or Distribution) and build using `Ctrl` + `Shift`+ `B`, or by navigating to `Build > Build Solution`.

### *Running:*

To test in the VS2019 environment, simply click "Local Windows Debugger" at the top, or press `F5` (`Ctrl` + `F5` if you want to start without the debugger).

## **Building for Raspberry Pi 3**

### *Requirements:*

- Latest version of Raspbian Stretch. For instructions on how to update your Raspberry Pi to Raspbian Stretch, follow [this official guide](https://www.raspberrypi.org/documentation/raspbian/updating.md).

- Build essentials and developer tools:

    ```sh
    build-essential git cmake pkg-config
    ```

- Developer image and video libraries:

    ```sh
    libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
    libxvidcore-dev libx264-dev
    ```

- GTK GUI backend:

    ```sh
    libgtk-3-dev libcanberra-gtk*
    ```

- Numerical optimization libraries:

    ```sh
    libatlas-base-dev gfortran
    ```

- USB device access:

    ```sh
    libusb-1.0-0-dev
    ```

**NOTE:** Running the script below with `--install-dependencies` will install and/or update these dependencies on your system automatically.

### *Recommended pre-build setup:*

Before building it is STRONGLY recommended to do perform the following procedures. If you believe these are not necessary or you have already performed these procedures, skip to [Building](#Building-for-Raspberry-Pi:).

- If you're using a brand-new install of Raspbian Stretch, expand your filesystem. Open a terminal window and type:

    ```sh
    $ sudo raspi-config
    ```

    Navigate to `7 Advanced Options`, and select `A1 Expand Filesystem`. Once you've done this, reboot your Raspberry Pi:

    ```sh
    $ sudo reboot
    ```

    **NOTE:** On a NOOBS install of Raspbian, this step is already done for you.

- If you don't need them, remove Wolfram and Libre Office from your system. This is especially useful on systems with low capacity SD cards. Removing these two programs will restore ~1 GB of space to your Raspberry Pi.

    ```sh
    $ sudo sudo apt-get purge wolfram-engine
    $ sudo apt-get purge libreoffice*
    $ sudo apt-get clean
    $ sudo apt-get autoremove
    ```

- Increase the swap. This procedure will create a swap file that is about 2x the size of your Raspberry Pi's RAM. Open a terminal and type:

    ```sh
    $ sudo nano /etc/dphys-swapfile
    ```

    Uncomment the following lines:

    ```nano
    CONF_SWAPFILE=/var/swap
    [...]
    CONF_SWAPFACTOR=2
    ```

    Then comment the line:

    ```nano
    #CONF_SWAPSIZE=100
    ```

    Press `Ctrl` + `X` to exit, `Y` to save changes, and `Enter` to confirm. Then in the terminal, type:

    ```sh
    $ sudo dphys-swapfile setup
    $ sudo dphys-swapfile swapon
    ```

    to restart the service and use the increased swap size.

### *Building for Raspberry Pi:*

1. Open a terminal window and input the following (`<Thermocheck repo location>` being the location where you cloned this repo to):

    ```sh
    $ cd <Thermocheck repo location>/Externals/opencv
    # --install-dependencies will install and/or upgrade all necessary dependencies automatically
    # Make sure to have sudo privileges if using this switch (the "pi" user should have this by default)
    $ ./install-opencv-rpi.sh [--install-dependencies]
    ```

    You can have a coffee or two while this builds. Maybe even three. Or four. This will take a while.

2. Navigate to the libuvc folder and run `install-libuvc.sh`:

    ```sh
    $ cd <Thermocheck repo location>/Externals/libuvc
    $ ./install-libuvc.sh
    ```

    Make sure libusb-1.0-0-dev is installed before you run this, or else it won't work.

3. Navigate to the root of the project and run the `Generate_GMake_Files.sh`:

    ```sh
    $ cd <Thermocheck repo location>
    $ ./Generate_GMake_Files.sh
    ```

    This will generate all the necessary Makefiles for the project

4. Build the project (using GCC):

    ```sh
    $ make -j3 -k                        # For debug builds
    $ make config=release -j3 -k         # For release builds
    $ make config=distribution -j3 -k    # For distribution builds
    ```

### *Running:*

Navigate to `Thermocheck/Build/bin/ARM/{config}` and run the Thermocheck executable.

## **Current build**

ImGui window with options, log, and video feed viewer.

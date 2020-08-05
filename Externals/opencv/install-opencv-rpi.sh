#!/usr/bin/env bash
CURRENT_LOCATION=$(pwd)
CMAKE_CONFIG_GENERATOR="Unix Makefiles"
OPENCV_INSTALL_LOCATION="/usr/local"
CMAKE_OPTIONS="\
-D BUILD_DOCS=OFF \
-D BUILD_EXAMPLES=OFF \
-D BUILD_PERF_TESTS=OFF \
-D BUILD_TESTS=OFF \
-D ENABLE_NEON=ON \
-D ENABLE_VFPV3=ON \
-D INSTALL_CREATE_DISTRIB=ON \
-D INSTALL_C_EXAMPLES=OFF \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D OPENCV_ENABLE_NONFREE=ON"

echo -e "\nInstalling/updating dependencies..."
sudo apt-get -y install \
git cmake build-essential pkg-config \
libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev \
libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
libtbb2 libtbb-dev \
libxvidcore-dev libx264-dev \
libgtk-3-dev libcanberra-gtk* \
libatlas-base-dev gfortran \
python3-dev python3-numpy python-dev python-numpy

echo -e "\nUpdating and upgrading..."
sudo apt-get -y update
sudo apt-get -y upgrade

if [ ! -d "$CURRENT_LOCATION/opencv" ]; then
	echo -e "\nCloning latest opencv commit..."
	git clone https://github.com/opencv/opencv.git opencv
else
	echo -e "\nChecking for opencv source updates..."
	cd $CURRENT_LOCATION/opencv
	git pull --rebase
	cd $CURRENT_LOCATION
fi

if [ ! -d "$CURRENT_LOCATION/opencv_contrib" ]; then
	echo -e "\nCloning latest opencv_contrib commit..."
	git clone https://github.com/opencv/opencv_contrib.git opencv_contrib
else
	echo -e "\nChecking for opencv_contrib source updates..."
	cd $CURRENT_LOCATION/opencv_contrib
	git pull --rebase
	cd $CURRENT_LOCATION
fi

if [ ! -d "$CURRENT_LOCATION/opencv/build" ]; then
	echo -e "\nMaking new opencv build location..."
	mkdir -p $CURRENT_LOCATION/opencv/build
fi

if [ ! -d "$CURRENT_LOCATION/opencv_contrib/build" ]; then
	echo -e "\nMaking new opencv_contrib build location..."
	mkdir -p $CURRENT_LOCATION/opencv_contrib/build
fi

echo -e "\n*****\nBuilding opencv...\n*****"
pushd $CURRENT_LOCATION/opencv/build
	cmake -G "$CMAKE_CONFIG_GENERATOR" $CMAKE_OPTIONS -D OPENCV_EXTRA_MODULES_PATH="$CURRENT_LOCATION/opencv_contrib/modules" -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX="$OPENCV_INSTALL_LOCATION" "$CURRENT_LOCATION/opencv"
	make -j3 -k
popd
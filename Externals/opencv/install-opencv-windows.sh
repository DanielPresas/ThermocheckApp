#!/usr/bin/env bash
CURRENT_LOCATION=$(pwd)
CMAKE_CONFIG_GENERATOR="Visual Studio 16 2019"
OPENCV_INSTALL_LOCATION="$CURRENT_LOCATION/install/opencv"
CMAKE_OPTIONS="\
-D BUILD_DOCS=OFF \
-D BUILD_EXAMPLES=OFF \
-D BUILD_PERF_TESTS=OFF \
-D BUILD_SHARED_LIBS=OFF \
-D BUILD_TESTS=OFF \
-D INSTALL_CREATE_DISTRIB=ON \
-D INSTALL_C_EXAMPLES=ON \
-D INSTALL_PYTHON_EXAMPLES=ON \
-D OPENCV_ENABLE_NONFREE=ON \
-D WITH_CUDA=OFF"

if [ ! -d "$CURRENT_LOCATION/opencv" ]; then
	echo -e "\nCloning latest opencv commit..."
	git clone https://github.com/opencv/opencv.git opencv
else
	echo -e "\nChecking for opencv source updates..."
	cd "$CURRENT_LOCATION/opencv"
	git pull --rebase
	cd "$CURRENT_LOCATION"
fi

if [ ! -d "$CURRENT_LOCATION/opencv_contrib" ]; then
	echo -e "\nCloning latest opencv_contrib commit..."
	git clone https://github.com/opencv/opencv_contrib.git opencv_contrib
else
	echo -e "\nChecking for opencv_contrib source updates..."
	cd "$CURRENT_LOCATION/opencv_contrib"
	git pull --rebase
	cd "$CURRENT_LOCATION"
fi

if [ ! -d "$CURRENT_LOCATION/opencv/build" ]; then
	echo -e "\nMaking new opencv build location..."
	mkdir -p "$CURRENT_LOCATION/opencv/build"
fi

if [ ! -d "$CURRENT_LOCATION/opencv_contrib/build" ]; then
	echo -e "\nMaking new opencv_contrib build location..."
	mkdir -p "$CURRENT_LOCATION/opencv_contrib/build"
fi

echo -e "\n*****\nBuilding opencv...\n*****"
pushd "$CURRENT_LOCATION/opencv/build"
	cmake -G "$CMAKE_CONFIG_GENERATOR" $CMAKE_OPTIONS -D OPENCV_EXTRA_MODULES_PATH="$CURRENT_LOCATION/opencv_contrib/modules" -D CMAKE_INSTALL_PREFIX="$OPENCV_INSTALL_LOCATION" "$CURRENT_LOCATION/opencv"

	echo -e "\nStarting debug build..."
	cmake --build .  --config debug
	
	echo -e "\nStarting release build..."
	cmake --build .  --config release
	
	if [ ! -d "$OPENCV_INSTALL_LOCATION" ]; then
		echo -e "\nMaking installation directory \"$OPENCV_INSTALL_LOCATION\"..."
		mkdir -p $OPENCV_INSTALL_LOCATION
	fi

	echo -e "\nInstalling libraries in $OPENCV_INSTALL_LOCATION..."
	cmake --build .  --target install --config debug
	cmake --build .  --target install --config release
popd
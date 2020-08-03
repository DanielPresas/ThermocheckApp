#!/usr/bin/env bash

REPO_LOCATION=$(pwd)
REPO_SOURCE=opencv
CMAKE_CONFIG_GENERATOR="Unix Makefiles"
CMAKE_OPTIONS='-DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=OFF -DBUILD_DOCS:BOOL=OFF -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=OFF -DINSTALL_CREATE_DISTRIB=ON -DBUILD_SHARED_LIBS=OFF, -DBUILD_WITH_STATIC_CRT=ON'

if [  ! -d "$REPO_LOCATION/opencv"  ]; then
    echo "Cloning opencv..."
    git clone https://github.com/opencv/opencv.git
    mkdir -p Build/opencv
    mkdir -p Install/opencv
else
    cd opencv
    git pull --rebase
    cd ..
fi
if [  ! -d "$REPO_LOCATION/opencv_contrib"  ]; then
    echo "Cloning opencv_contrib..."
    git clone https://github.com/opencv/opencv_contrib.git
    mkdir -p Build/opencv_contrib
else
    cd opencv_contrib
    git pull --rebase
    cd ..
fi

pushd Build/$REPO_SOURCE

	echo "************************* $Source_DIR -->Debug"
	cmake -G "$CMAKE_CONFIG_GENERATOR" $CMAKE_OPTIONS -DCMAKE_BUILD_TYPE=Debug -DOPENCV_EXTRA_MODULES_PATH="$REPO_LOCATION"/opencv_contrib/modules -DCMAKE_INSTALL_PREFIX="$REPO_LOCATION"/install/"$REPO_SOURCE" "$REPO_LOCATION/$REPO_SOURCE"
	cmake --build .  --config debug -- -j4
	cmake --build .  --target install --config debug -- -j4

	echo "************************* $Source_DIR -->Release"
	cmake -G "$CMAKE_CONFIG_GENERATOR" $CMAKE_OPTIONS -DCMAKE_BUILD_TYPE=Release -DOPENCV_EXTRA_MODULES_PATH="$REPO_LOCATION"/opencv_contrib/modules -DCMAKE_INSTALL_PREFIX="$REPO_LOCATION"/install/"$REPO_SOURCE" "$REPO_LOCATION/$REPO_SOURCE"
		
	cmake --build .  --config release -- -j4
	cmake --build .  --target install --config release -- -j4

popd

cmake -G "Unix Makefiles" \
-D BUILD_DOCS=OFF \
-D BUILD_EXAMPLES=OFF \
-D BUILD_PERF_TESTS=OFF \
-D BUILD_TBB=ON \
-D BUILD_TESTS=OFF \
-D BUILD_SHARED_LIBS=OFF \
-D ENABLE_NEON=ON \
-D ENABLE_VFPV3=ON \
-D INSTALL_CREATE_DISTRIB=ON \
-D INSTALL_C_EXAMPLES=ON \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=~/repos/Thermocheck/Externals/opencv/opencv_contrib/modules \
-D OPENCV_FORCE_3RDPARTY_BUILD=ON \
-D WITH_CUDA=OFF \
-D WITH_OPENGL=ON \
-D WITH_TBB=ON \
-D WITH_V4L=ON \
-D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=~/repos/Thermocheck/Externals/opencv/install/opencv \
../../opencv

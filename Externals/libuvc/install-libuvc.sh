mkdir -p build
pushd build

cmake ..
make -j3 -k

popd
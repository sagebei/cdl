mkdir build

# compile and install the library
(cd build && cmake ..)
(cd build && make)

./build/tool
# remove the unneeded build directory
rm -rf "./build"







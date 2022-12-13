mkdir build

# compile and install the library
(cd build && cmake ..)
(cd build && make)

./build/tools
# remove the unneeded build directory
rm -rf "./build"







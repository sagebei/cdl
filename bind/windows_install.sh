mkdir build
cd build
cmake ..

cmake --build . --target ALL_BUILD --config Release

mv ./Release/cdl.cp38-win_amd64.pyd D:\Anaconda3\Lib\site-packages\

cd ..

rm -r build

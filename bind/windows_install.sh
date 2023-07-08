mkdir build
cd build
cmake ..

cmake --build . --target ALL_BUILD --config Release
#rm /D/Anaconda3/Lib/site-packages/cdl.cp38-win_amd64.pyd

mv -f ./Release/cdl.cp38-win_amd64.pyd /D/Anaconda3/Lib/site-packages/

cd ..

rm -r build

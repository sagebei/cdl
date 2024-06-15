
if [ -z "$1" ]
  then
    echo "You need to supply the path to virtual environment in which CDL will be installed, for example, bash windows_install.sh D:/Anaconda3/Lib/site-packages/"
    exit 1
fi
# bash window_install.sh D:/Anaconda3/Lib/site-packages/

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

git pull

mkdir build
cd build
cmake ..

cmake --build . --target ALL_BUILD --config Release
#rm /D/Anaconda3/Lib/site-packages/cdl.cp38-win_amd64.pyd

mv -f ./Release/cdl.*.pyd $1
pwd
cd ..

rm -r build

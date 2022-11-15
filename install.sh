rm -rf build
mkdir build

python_dir=$(which python)
dir=$(python -m site --user-site)

(cd build && cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DPYTHON_EXECUTABLE=$python_dir ../)
(cd build && cmake ..)
(cd build && make)

if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && mv *.so $dir)



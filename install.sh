rm -rf build
mkdir build

python_dir=$(which python)
(cd build && cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DPYTHON_EXECUTABLE=$python_dir ../)

(cd build && cmake ..)
(cd build && make)

dir=$(python -m site --user-site)
if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && mv *.so $dir)
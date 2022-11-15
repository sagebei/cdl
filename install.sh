rm -rf build
mkdir build

python_dir=$(which python)
dir=$(python -m site --user-site)

if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=$python_dir ../)
(cd build && cmake ..)
(cd build && make)
(cd build && mv *.so $dir)



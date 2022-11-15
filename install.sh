if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

python_dir=$(which python)
dir=$(python -m site --user-site)

if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DPYTHON_EXECUTABLE=$python_dir ../)
(cd build && cmake ..)
(cd build && make)
(cd build && mv *.so $dir)





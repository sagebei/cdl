rm -rf build
mkdir build
(cd build && cmake ..)
(cd build && make)

dir=$(python -m site --user-site)
if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && mv *.so $dir)
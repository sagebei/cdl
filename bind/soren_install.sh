export CC=/usr/bin/gcc
export CXX=/usr/bin/g++

rm -rf build
mkdir build
(cd build && cmake -DPYTHON_EXECUTABLE=~/opt/anaconda3/bin/python ..)
(cd build && make)

rm ~/opt/anaconda3/lib/python3.7/site-packages/cdl*
mv cdl* ~/opt/anaconda3/lib/python3.7/site-packages

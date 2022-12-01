if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

if [ $(uname) == "Darwin" ]; then
  export CC=/usr/bin/gcc
  export CXX=/usr/bin/g++
fi

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

export PYTHONUSERBASE=$1
source $PYTHONUSERBASE/bin/activate

SITE_PACKAGES=$(python -c 'import site; print(site.getsitepackages()[0])')
echo $SITE_PACKAGES
if [ ! -d $SITE_PACKAGES ]; then
  mkdir $SITE_PACKAGES
fi

# removing the exising outdated library
rm $SITE_PACKAGES/cdl*

(cd build && cmake -DPYTHON_EXECUTABLE=$PYTHONUSERBASE/bin/python ..)
(cd build && make)
(cd build && mv *.so $SITE_PACKAGES)

rm -rf "./build"





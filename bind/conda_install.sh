git pull

if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

# locate the SITE_PACKAGE folder
module load anaconda3/2021.11

export PYTHONUSERBASE=$1
conda activate $PYTHONUSERBASE
SITE_PACKAGES=$(python -c 'import site; print(site.getsitepackages()[0])')
echo $SITE_PACKAGES
if [ ! -d $SITE_PACKAGES ]; then
  mkdir $SITE_PACKAGES
fi

# compile and install the library
(cd build && cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=$PYTHONUSERBASE/bin/python ..)
(cd build && make)

# removing the exising outdated library before installing the new one.
rm $SITE_PACKAGES/cdl*
(cd build && mv *.so $SITE_PACKAGES)

# remove the unneeded build directory
rm -rf "./build"





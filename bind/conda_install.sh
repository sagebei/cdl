git pull

module load cmake
module load gcc/12.1.0
module load anaconda3/2022.05
conda activate /data/home/acw554/cvenv/

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

SITE_PACKAGES=/data/home/acw554/cvenv/lib/python3.10/site-packages
# compile and install the library
(cd build && cmake -DCMAKE_BUILD_TYPE=Release -DPYTHON_EXECUTABLE=/data/home/acw554/cvenv/bin/python ..)
(cd build && make)

# removing the exising outdated library before installing the new one.
rm $SITE_PACKAGES/cdl*
(cd build && mv *.so $SITE_PACKAGES)

# remove the unneeded build directory
rm -rf "./build"





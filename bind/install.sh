if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

#
OS_NAME=$(uname)
if [[ $OS_NAME == "Darwin" ]]; then
  export PYTHONUSERBASE='/opt/anaconda3'             # macos
elif [[ $OS_NAME == "Linux" ]]; then
  export PYTHONUSERBASE=$HOME"/PycharmProjects/venv"
fi
echo $PYTHONUSERBASE

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

SITE_PACKAGES=$(python -m site --user-site)
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





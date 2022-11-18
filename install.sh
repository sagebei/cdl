if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

# change this environment variable to your virtual environment directory, like $HOME'/venv'
#export PYTHONUSERBASE='/opt/anaconda3'             # macos
#export PYTHONUSERBASE=$HOME'/venv'                 # hpc
export PYTHONUSERBASE=$HOME"/PycharmProjects/venv"  # ubuntu

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

dir=$(python -m site --user-site)
echo $dir
if [ ! -d $dir ]; then
  mkdir $dir
fi

(cd build && cmake ..)
(cd build && make)
(cd build && mv *.so $dir)

rm -rf "./build"





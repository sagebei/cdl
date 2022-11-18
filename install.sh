if [ -d "./build" ]; then
  rm -rf "./build"
  mkdir "./build"
else
  mkdir "./build"
fi

# change this environment variable to your virtual environment directory, like "~/venv"
#export PYTHONUSERBASE='/opt/anaconda3'
#export PYTHONUSERBASE=$HOME'/venv'
export PYTHONUSERBASE=$HOME"/PycharmProjects/venv"

if [ ! -d "./pybind11" ]; then
  git clone https://github.com/pybind/pybind11.git
fi

dir=$(python -m site --user-site)

if [ ! -d $dir ]; then
  mkdir $dir
fi

echo $dir

(cd build && cmake ..)
(cd build && make)
(cd build && mv *.so $dir)

rm -rf "./build"





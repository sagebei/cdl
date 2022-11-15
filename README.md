# cd

|Latest Documentation Status| |Stable Documentation Status| |Gitter chat| |GitHub Discussions| |CI| |Build status|

|Repology| |PyPI package| |Conda-forge| |Python Versions|


## Deal with system issues

1. Imported target "pybind11::module" includes non-existent path
- `cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=/opt/anaconda3/bin/python ../`
2. Enable importing the library from global scale
- mkdir -p "\`python -m site --user-site\`"
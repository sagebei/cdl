# Condorcet Domain Library 

Condorcet Domain Library (CDL) provides all the functionalities pertaining to 
creating and manipulating triplet-rules and condorcet domains (CD), including
- assign rule to the triplets by rule scheme
- fast and efficient construction of a CD given a set of triplet-rules
- Closing operations: given a CD, find its corresponding triplet-rules
- etc. 

CDL is a header-only library writen in C++ and offers Python Interfaces as a module that can be installed and 
used globally, enabling users to seamlessly integrate with tools writen in Python. 


|Latest Documentation Status| |Stable Documentation Status| |Gitter chat| |GitHub Discussions| |CI| |Build status|

|Repology| |PyPI package| |Conda-forge| |Python Versions|


## Deal with system issues

1. Imported target "pybind11::module" includes non-existent path
- `cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=/opt/anaconda3/bin/python ../`
2. Enable importing the library from global scale
- mkdir -p "\`python -m site --user-site\`"
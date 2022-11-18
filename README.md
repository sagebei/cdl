# Condorcet Domain Library 

[![Version](https://img.shields.io/badge/Version-1.1-green.svg)]()
[![Python 3.8](https://img.shields.io/badge/Python-3.6+-blue.svg)](https://www.python.org/downloads/release/python-380/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)]()
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](./license)

Condorcet Domain Library (CDL) provides all the functionalities pertaining to 
creating and manipulating triplet-rules and condorcet domains (CD), including
- assign rule to the triplets by rule scheme
- fast and efficient construction of a CD given a set of triplet-rules
- Closing operation: given a CD, find its corresponding triplet-rules
- Hash a set of brother CDs such that all the CDs in it have the same hash number. 
- etc.

CDL is a header-only library writen in C++ and offers Python Interfaces as a module that can be
installed and used globally, enabling users to seamlessly integrate with tools writen in Python.

## Get started with C++
```c++
#include "condorcet_domain.h"

int main()
{
    CondorcetDomain cd(5);
    RuleScheme rs;
    rs.add({2, 3}, "3N1");
    rs.add({4}, "1N3");

    TRS trs = cd.initialize_by_scheme(rs);
    CD domain = cd.condorcet_domain(trs);
    cd.print_cd(domain);
    
    CDS bros = cd.cd_brothers(domain);
    for (const CD& bro: bros)
    {
        cd.print_cd(bro);
    }
    
    return 0;
}
```

## Get started with Python
```python
from cdl import CondorcetDomain, RuleScheme

cd = CondorcetDomain(n=5)
rs = RuleScheme()
rs.add([2, 3], "3N1")
rs.add([4], "1N3")

trs = cd.initialize_by_scheme(rs)
domain = cd.condorcet_domain(trs)
print("Domain size: ", len(domain))
cd.print_cd(domain)

bros = cd.cd_brothers(domain)  # bros include domain itself
for bro in bros:
   cd.print_cd(bro)

# brother cds have the same hash value
for bro in bros:
   cds = cd.cd_brothers(bro)
   print('Bros Hash: ', cd.hash_cd_brothers(cds))
```

## Installation for Python Program

1. Open a terminal and change working directory to the `cdl` folder
2. Install `Python`, `gcc`, `cmake` if you have not
3. In `install.sh`, Edit the `PYTHONUSERBASE` variable to your virtual environment directory
in which you will use the library. 
4. Run `bash install.sh`. This will download `pybind11` libray that is essential to compile the code,
   and install the `dgl` library to the site-package folder in the virtual environment such that you can import it in any Python program.


If you enconter any issues during the installation process, please raise an issue in this repository. Common issues are:
1. Imported target "pybind11::module" includes non-existent path. This is because the cmake cannot find the python installation.
It can be fixed by running `cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=/opt/anaconda3/bin/python ../`
where `/opt/anaconda3/bin/python` should be changed tp the path to your Python installation. 


## Cite
Please cite our paper that introduced CDL library if you use it in a scientific publication. 


## Our Team
CDL is developed and maintained by Bei Zhou and Dr.Soren Riis 
in Theory Group and Game AI group at Queen Mary University of London. 






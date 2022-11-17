# Condorcet Domain Library 

Condorcet Domain Library (CDL) provides all the functionalities pertaining to 
creating and manipulating triplet-rules and condorcet domains (CD), including
- assign rule to the triplets by rule scheme
- fast and efficient construction of a CD given a set of triplet-rules
- Closing operation: given a CD, find its corresponding triplet-rules
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
cd.print_cd(domain)

bros = cd.cd_brothers(domain);
for bro in bros:
    cd.print_cd(bro)

```

## Installation

1. Imported target "pybind11::module" includes non-existent path
- `cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=/opt/anaconda3/bin/python ../`
2. Enable importing the library from global scale
- mkdir -p "\`python -m site --user-site\`"


## Cite
Please cite our paper that introduced CDL library if you use it in a scientific publication. 


## Our Team
DGL is developed and maintained by Bei Zhou and Dr.Soren Riis 
in Theory Group and Game AI group at Queen Mary University of London. 






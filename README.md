# Condorcet Domain Library 

Condorcet Domain Library (CDL) provides all the functionalities pertaining to 
creating and manipulating triplet-rules and condorcet domains (CD), including
- assign rule to the triplets by rule scheme
- fast and efficient construction of a CD given a set of triplet-rules
- Closing operations: given a CD, find its corresponding triplet-rules
- etc. 

CDL is a header-only library writen in C++ and offers Python Interfaces as a module that can be
installed and used globally, enabling users to seamlessly integrate with tools writen in Python. 


|Latest Documentation Status| |Stable Documentation Status| |Gitter chat| |GitHub Discussions| |CI| |Build status|

|Repology| |PyPI package| |Conda-forge| |Python Versions|


## How to use CDL in C++
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

## How to use CDL in Python
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


## Deal with system issues

1. Imported target "pybind11::module" includes non-existent path
- `cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=/opt/anaconda3/bin/python ../`
2. Enable importing the library from global scale
- mkdir -p "\`python -m site --user-site\`"





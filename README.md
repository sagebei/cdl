# Condorcet Domain Library 

[![Version](https://img.shields.io/badge/Version-1.1-green.svg)]()
[![Python 3.8](https://img.shields.io/badge/Python-3.6+-blue.svg)](https://www.python.org/downloads/release/python-380/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)]()
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](./license)

Condorcet Domain Library (CDL) is a flexible header-only library writen in C++ and offers Python Interfaces as a module that can be
installed and used globally, enabling users to seamlessly integrate with tools writen in Python.  (CDL)
provides a wide range of functionalities pertaining to Condorcet Domains (CD) and forbidden permutation domains, including
- triple ordering, and rule initialization and assignment
- Domain construction and size calculation
- Subset Functions
- Hashing and identifying non-isomorphic CDs
- Native support for any forbidden permutation domains
- Calculate the size of any the subset CDs.
- Supported 6 rules: `1N3`, `3N1`, `2N3`, `2N1`, `1N2` and `3N2`.
- etc.

CDL supports all major operating systems, like Windows, Linux and MacOS. User can install it as a python module using 
the provided bash scripts. 

Directory structure:
- algorithms: 
- bind: export all the functionality to a python module and provide the bash script install it. 
- core: the key functionality for manipulating triple-rules and condorcet domains.
- python: 
- tools: 

Brief introduction to the Condorcet Domain.

<p align="center">
  <img src="https://www.parisschoolofeconomics.eu/local/cache-vignettes/L690xH373/2-5-et-5-pse-mai-2021-xl-f57ef.png"
        style="width:350px;">
</p>

## Get started with Python
### working with Condorcet domains
```python
from cdl import *

def alternating_scheme(triplet):  # build the alternating scheme 
   i, j, k = triplet
   if j % 2 == 0:
      return "2N1"
   else:
      return "2N3"

cd = CondorcetDomain(n=8)  # initialize the Condorcet domain object
# initialize the trs with the predefined alternating scheme 
trs = cd.init_trs_by_scheme(alternating_scheme)
domain = cd.domain(trs)  # construct the Condorcet domain
size = cd.size(trs)  # calculate the size of the resulting Condorcet domain (222)
assert len(domain) == size  # True

# change the rule assigned to the triplet [2, 3, 4] from "2N3" to "3N1"
trs = cd.assign_rule(trs, [2, 3, 4], "3N1")
size = cd.size(trs) # the size of the new domain is 210.

cd.init_subset(sub_n=6)
substates = cd.subset_states(trs) # get a list of 28 subset states in 6 alternatives

# build a list of domains
domains = [cd.domain(cd.init_trs_random()) for _ in range(100)]
# filter out the isomorphic domains
non_isomorphic_cds = cd.non_isomorphic_domains(domains)  
```
### working with Forbidden Permutations
```python
# recreate the alternating scheme by forbidden permutations
def alternating_scheme(triple):  
    i, j, k = triple
    if j % 2 == 0:
        return ["213", "231"]
    else:
        return ["132", "312"]

fp = ForbiddenPermutation(n=8)  # initialize the Forbidden Permutation object
tls = fp.init_tls_by_scheme(alternating_scheme)
domain = fp.domain(tls)
size = fp.size(tls)  # 222
assert len(domain) == size
```

## Installation for Python Program
### Install CDL for Linux or MacOS
1. Open a terminal and change working directory to the `cdl` folder
2. Install `Python3` or `anaconda`, `gcc`, `cmake` if you have not
3. Run `source install.sh \path\to\your\virtural_environment` to install 
   the library to an existing virtual environment in which you will import it.
   (This will download `pybind11` libray that is essential to compile the code,
   and install the `dgl` library to the site-package folder in the virtual environment.
   <br />
   Examples: `source install.sh \opt\anaconda3` to install the library in the anaconda global environment, 
   or `source install.sh ~\PyCharmProjects\venv` to install it in a virtual environment
   created in the PycharmProjects directory.) 

### Install CDL for Windows
1. Install `git`, `Python3` or `anaconda`, `gcc`, `cmake` if you have not
2. Open a Git Bash terminal, and change working directory to `cdl/bind`
3. Run `source windows_install.sh \path\to\your\virtural_environment`. For example, 
   `source windows_install.sh /D/Anaconda3/Lib/site-packages/`

## Get started with C++
### working with Condorcet domains
```c++
#include "condorcet_domain.h"

std::string alternating_scheme(const Triple& triple)
{
    if ((triple[1] % 2) == 0)
        return "2N1";
    else
        return "2N3";
}

int main()
{
    CondorcetDomain cd(6);
    auto trs = cd.init_trs_by_scheme(alternating_scheme);
    std::cout << (cd.size(trs) == cd.domain(trs).size()) << std::endl;

    CD domain = cd.domain(trs);
    CDS domains{};
    domains.push_back(domain);
    domains.push_back(domain);

    CDS new_cds = cd.non_isomorphic_domains(domains);
    return 0;
}

```
### working with Forbidden permutations
```c++
#include "forbidden_permutation.h"

std::vector<std::string> alternating_scheme(const Triple& triple)
{
    if ((triple[1] % 2) == 0)
        return {"213", "231"};
    else
        return {"132", "312"};
}

int main()
{
    ForbiddenPermutation fp(8);
    TLS tls = fp.init_tls_by_scheme(alternating_scheme);
    std::cout << (fp.size(tls) == fp.domain(tls).size()) << std::endl;
    return 0;
}
```

## Cite
Please cite our paper that introduced CDL library if you use it in a scientific publication. 


## List of publications using CDL

- Zhou, Bei, and Søren Riis. "New Record-Breaking Condorcet Domains on 10 and 11 Alternatives." arXiv preprint arXiv:2303.06524 (2023).

## Our Team
CDL is developed and maintained by Bei Zhou and Dr Soren Riis 
in Theory Group and Game AI group at Queen Mary University of London. 






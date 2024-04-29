# Condorcet Domain Library 

[![Version](https://img.shields.io/badge/Version-1.1-green.svg)]()
[![Python 3.8](https://img.shields.io/badge/Python-3.6+-blue.svg)](https://www.python.org/downloads/release/python-380/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)]()
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](./license)

[Condorcet Domain Library (CDL)](https://arxiv.org/pdf/2309.06306.pdf) is a flexible header-only library written in C++ and offers Python Interfaces as a module that can be
installed and used globally, enabling users to seamlessly integrate with tools written in Python.  (CDL)
provides a wide range of functionalities pertaining to Condorcet Domains (CD) and forbidden permutation, including
- Ordering k-tuples, and rule initialization and assignment;
- Domain construction and size calculation;
- Subset functions and domain types verification;
- Hashing, identifying and removing non-isomorphic domains;
- Native support for general forbidden permutation domains;
- Support all 6 rules: `1N3`, `3N1`, `2N3`, `2N1`, `1N2` and `3N2`;
- and much more.

CDL supports all major operating systems, including Windows, Linux and MacOS. Users can install it as a python module using 
the provided bash scripts. 

Directory structure:
- algorithms: for testing and benchmarking many learning algorithms, like genetic algorithms, reinforcement learning 
algorithms, and local search algorithms, etc.
- bind: export all the C++ classes and functions to a python module and provide the bash script install it. 
- core: the key functionality for manipulating tuple-rules and performing domain-related operations.
- python: provide depth-first and breast-first Prioritised Restriction Search (PRS) search algorithms
- utils: provide functionss  


## Get started with Python
### Working with Condorcet domains
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
### Working with Forbidden Permutations
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


```python
from cdl import ForbiddenPermutation

for n in range(5, 11):
    # initialize the ForbiddenPermutation object for 5-tuples
    fp = ForbiddenPermutation(n, 5) 
    tls = fp.init_tls()
    for tl in tls:
        # assign all the 5-tuples with the law [2, 5, 3, 1, 4]
        tls = fp.assign_laws(tls, tl.tuple, [[2, 5, 3, 1, 4]])
    print(fp.size(tls))
```

## Installation for Python Program
### Install CDL for Linux or MacOS
1. Open a terminal and download the CDL repository to your laptop by `git clone https://github.com/sagebei/cdl.git`
2. Change working directory to the `cdl/bind` folder
3. Install `Python3` or `anaconda`, `gcc`, `cmake` if you have not. You might need to load the gcc
   and the cmake module by running `module load gcc cmake` if you are using a server machine. 
4. - Installing to an existing Python virtual environment: Run `source install.sh \path\to\your\virtural_environment` to install 
   the library to an existing virtual environment in which you will import it.
   (This will download `pybind11` libray that is essential to compile the code,
   and install the `dgl` library to the site-package folder in the virtual environment.
   Examples: `source install.sh \opt\anaconda3` to install the library in the anaconda global environment, 
   or `source install.sh ~\PyCharmProjects\venv` to install it in a virtual environment
   created in the PycharmProjects directory.) 
   - Creat a new virtual environment: `python -m venv /path/to/new/virtual/environment`. Then follow the above
      instructions to install the CDL library in it. 

### Install CDL for Windows
1. Install `git`, `Python3` or `anaconda`, `gcc`, `cmake` if you have not. You might need to load the `gcc`
   and the `cmake` module by running `module load gcc cmake` if you are using a server machine.
2. Open a Git Bash terminal, and change working directory to `cdl/bind`
3. Run `source windows_install.sh \path\to\your\virtural_environment`. For example, 
   `source windows_install.sh /D/Anaconda3/Lib/site-packages/`

## Get started with C++
### Working with Condorcet domains
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
### Working with Forbidden permutations
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
Please cite our paper if you use CDL in a scientific publication.
```
@article{zhou2023cdl,
  title={CDL: A fast and flexible library for the study of permutation sets with structural restrictions},
  author={Zhou, Bei and Markstr{\=o}m, Klas and Riis, S{\o}ren},
  journal={arXiv preprint arXiv:2309.06306},
  year={2023}
}
```

## List of publications used CDL

1. Zhou, Bei, and Søren Riis. "New Record-Breaking Condorcet Domains on 10 and 11 Alternatives." arXiv preprint arXiv:2303.06524 (2023).
2. Akello-Egwell, Dolica, Charles Leedham-Green, Alastair Litterick, Klas Markström, and Søren Riis. "Condorcet Domains of Degree at most Seven." arXiv preprint arXiv:2306.15993 (2023).
3. Karpov, Alexander, Klas Markström, Søren Riis, and Bei Zhou. "Set-alternating schemes: A new class of large Condorcet domains." arXiv preprint arXiv:2308.02817 (2023).
4. Karpov, Alexander, Klas Markström, Søren Riis, and Bei Zhou. "Local Diversity of Condorcet Domains." arXiv preprint arXiv:2401.11912 (2024).
5. Markström, Klas, Søren Riis, and Bei Zhou. "Arrow's single peaked domains, richness, and domains for plurality and the Borda count." arXiv preprint arXiv:2401.12547 (2024).
## Our Team
CDL is developed and maintained by Dr Bei Zhou and Dr Soren Riis in the theory group at Queen Mary University of London, and Professor Klas Markstrom
from University of Umeå.






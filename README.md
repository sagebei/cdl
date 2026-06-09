<div align="center">

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="assets/cdl-logo-dark@2x.png">
  <img alt="CDL — Condorcet Domain Library" src="assets/cdl-logo@2x.png" width="460">
</picture>

<br>
<br>

**A fast and flexible library for the study of permutation sets with structural restrictions**


[![Python](https://img.shields.io/badge/Python-3.8%2B-4c8bf5.svg?logo=python&logoColor=white)](https://www.python.org/downloads/release/python-380/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-9b6bdf.svg?logo=c%2B%2B&logoColor=white)]()
[![Platforms](https://img.shields.io/badge/Platforms-Windows%20%7C%20macOS%20%7C%20Linux-2dd4bf.svg)]()
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](./license)
[![Paper](https://img.shields.io/badge/Paper-SoftwareX%202024-ff6f3c.svg)](https://www.sciencedirect.com/science/article/pii/S2352711024003212)

[**Installation**](#installation) · [**Quick Start**](#quick-start-python) · [**C++ API**](#c-api) · [**Repository Structure**](#repository-structure) · [**Citation**](#citation)

</div>

---

[**Condorcet Domain Library (CDL)**](https://www.sciencedirect.com/science/article/pii/S2352711024003212) is a lightweight, **header-only C++** library — with first-class **Python bindings** installable via `pip` — for studying **Condorcet Domains (CD)** and **forbidden permutations**. It is designed to integrate seamlessly into existing Python toolchains while retaining the performance of native C++.

## Features

- **Tuple ordering** — k-tuple ordering, with rule initialization and assignment.
- **Domain construction** — build domains and compute their size directly.
- **Never conditions** — find the complete set of *never conditions* that a domain satisfies.
- **Subsets & types** — subset functions and domain-type verification.
- **Isomorphism** — hash, identify, and generate non-isomorphic domains.
- **Forbidden permutations** — native support for general forbidden-permutation domains.
- **All six rules** — `1N3`, `3N1`, `2N3`, `2N1`, `1N2`, and `3N2`.
- **Cross-platform** — Windows, macOS, and all major Linux distributions.

## Installation

### Install with pip

```bash
pip install condorcet-domain
```

> [!NOTE]
> The PyPI package is named **`condorcet-domain`**, but the module imported in Python is **`cdl`**.
> See [CDL on PyPI](https://pypi.org/project/condorcet-domain/).

### Build from source — Linux / macOS

1. Clone the repository:
   ```bash
   git clone https://github.com/sagebei/cdl.git
   cd cdl/bind
   ```
2. Install `Python 3` (or Anaconda), `gcc`, and `cmake` if they are not already available.
   On a cluster you may first need to load the toolchain with `module load gcc cmake`.
3. Install into a Python environment:
   - **Existing environment** — install into a virtual environment you already use:
     ```bash
     source install.sh /path/to/your/virtual_environment
     ```
     The script downloads `pybind11` (required to compile the code) and installs the library into the
     environment's `site-packages`.
     For example, `source install.sh /opt/anaconda3` installs into the global Anaconda environment, and
     `source install.sh ~/PycharmProjects/venv` installs into a virtual environment under `PycharmProjects`.
   - **New environment** — create one first, then run the command above:
     ```bash
     python -m venv /path/to/new/virtual_environment
     ```

### Build from source — Windows

1. Install `git`, `Python 3` (or Anaconda), `gcc`, and `cmake` if they are not already available.
   On a cluster you may first need to load the toolchain with `module load gcc cmake`.
2. Open a **Git Bash** terminal and change into the `cdl/bind` directory.
3. Run the installer with the target environment path:
   ```bash
   source windows_install.sh /D/Anaconda3/Lib/site-packages/
   ```

## Quick Start (Python)

### Working with Condorcet Domains

```python
from cdl import *

def alternating_scheme(triple):  # build the alternating scheme
   i, j, k = triple
   if j % 2 == 0:
      return "2N1"
   else:
      return "2N3"

cd = CondorcetDomain(n=8)  # initialize the Condorcet domain object
# initialize the trs with the predefined alternating scheme
trs = cd.init_trs()
trs = cd.init_trs_by_scheme(trs, alternating_scheme)
domain = cd.domain(trs)  # construct the Condorcet domain
size = cd.size(trs)  # calculate the size of the resulting Condorcet domain (222)
assert len(domain) == size  # True

# change the rule assigned to the triplet [2, 3, 4] from "2N3" to "3N1"
trs = cd.assign_rule(trs, [2, 3, 4], "3N1")
size = cd.size(trs)  # the size of the new domain is 210.

cd.init_subset(sub_n=6)
substates = cd.subset_states(trs)  # get a list of 28 subset states in 6 alternatives

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
        return [[2, 1, 3], [2, 3, 1]]
    else:
        return [[1, 3, 2], [3, 1, 2]]

fp = ForbiddenPermutation(n=8, k=3)  # initialize the Forbidden Permutation object
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

> [!TIP]
> New to CDL? Start with the notebooks in [`tutorials/`](./tutorials) — `Get Started with CDL.ipynb` walks through the core workflow end to end.

## C++ API

CDL is header-only: include the relevant header and compile against the `core/include` directory.

### Working with Condorcet Domains

```cpp
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
    auto trs = cd.init_trs_by_scheme(cd.init_trs(), alternating_scheme);
    std::cout << (cd.size(trs) == cd.domain(trs).size()) << std::endl;

    CD domain = cd.domain(trs);
    CDS domains{};
    domains.push_back(domain);
    domains.push_back(domain);

    CDS new_cds = cd.non_isomorphic_domains(domains);
    return 0;
}
```

### Working with Forbidden Permutations

```cpp
#include "forbidden_permutation.h"

std::vector<std::string> alternating_scheme(const Triple& triple)
{
    if ((triple[1] % 2) == 0)
        return {{2, 1, 3}, {2, 3, 1}};
    else
        return {{1, 3, 2}, {3, 1, 2}};
}

int main()
{
    ForbiddenPermutation fp(8, 3);
    TLS tls = fp.init_tls_by_scheme(alternating_scheme);
    std::cout << (fp.size(tls) == fp.domain(tls).size()) << std::endl;
    return 0;
}
```

## Repository Structure

| Directory | Description |
| --- | --- |
| [`core/`](./core) | The header-only C++ engine: tuple–rule manipulation and all domain-related operations. |
| [`bind/`](./bind) | Pybind11 bindings that export the C++ classes and functions as a Python module, plus install scripts. |
| [`python/`](./python) | Depth-first and breadth-first Prioritised Restriction Search (PRS) algorithms. |
| [`orderly/`](./orderly) | Orderly generation, complete/parallel search, and LaTeX export utilities. |
| [`algorithms/`](./algorithms) | Learning-based domain search: genetic algorithms, reinforcement learning, local search, and benchmarks. |
| [`tools/`](./tools) | Utilities for processing [MUCDs](http://abel.math.umu.se/~klasm/Data/CONDORCET/MUCDS/), checking domain maximality, and parallel computation on median graphs. |
| [`hpc/`](./hpc) | Example Bash scripts for running CDL on High Performance Computing clusters. |
| [`tutorials/`](./tutorials) | Jupyter notebooks demonstrating the library, including a getting-started guide. |

## Citation

If you use CDL in a scientific publication, please cite our paper:

```bibtex
@article{zhou2024cdl,
  title   = {CDL: A fast and flexible library for the study of permutation sets with structural restrictions},
  author  = {Zhou, Bei and Markstr{\"o}m, Klas and Riis, S{\o}ren},
  journal = {SoftwareX},
  volume  = {28},
  pages   = {101951},
  year    = {2024},
  publisher = {Elsevier}
}
```

## Team

CDL is developed and maintained by:

- **Dr Bei Zhou** — Imperial College London
- **Dr Søren Riis** — Queen Mary University of London
- **Prof. Klas Markström** — University of Umeå

## License

Distributed under the **Apache License 2.0**. See [`license`](./license) for details.

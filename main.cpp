#include <iostream>
#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.initialize(false);
    auto size = cd.condorcet_domain(trs);
    std::cout << size << std::endl;
    return 0;
}


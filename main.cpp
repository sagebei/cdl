#include <iostream>
#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    RuleScheme rs;
    rs.add({1, 3, 4}, "1N3");
    rs.add({2, 5}, "2N1");

    TRS trs = cd.initialize_by_scheme(rs);
    cd.print_tr(trs);
    auto domain = cd.condorcet_domain(trs);
    std::cout << domain.size() << std::endl;
    return 0;
}


#include "condorcet_domain.h"

int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.init_trs();
    trs = cd.assign_rule(trs, {1, 2, 3}, "2N3");
    print_trs(trs);
    std::cout << cd.is_domain_copious(trs, cd.domain(trs)) << std::endl;

}


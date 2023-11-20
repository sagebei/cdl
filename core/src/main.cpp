#include "condorcet_domain.h"

int main()
{
    CondorcetDomain cd(5);
    auto trs = cd.init_trs();
    trs = cd.assign_rule_by_index(trs, 0, "1N3");
    trs = cd.assign_rule_by_index(trs, 1, "1N3");

    print_trs(trs);
    bool result = cd.is_trs_isomorphic(trs, {"1N3", "2N3"});
    std::cout<< result << std::endl;
    return 0;
}


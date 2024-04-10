#include "condorcet_domain.h"

int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.init_trs();
    trs = cd.assign_rule(trs, {1, 2, 3}, "2N3");
    print_trs(trs);
    std::cout << cd.isomorphic_trs_list(trs, {"1N3", "3N1", "2N3", "2N1"}).size() << std::endl;
//    print_trs(cd.inverse_trs(trs, {1, 2, 3, 5, 4}, {"1N3", "3N1", "2N3", "2N1"}));
}


#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd1(5);
    TRS trs = cd1.init_empty();
    CD cd = cd1.condorcet_domain(trs);
    TRS t = cd1.domain_to_trs(cd, true);

//    RuleScheme rs1;
//    rs1.add({2, 3, 4}, "1N3");
//    TRS large = cd1.init_empty();
//
//    CondorcetDomain cd2(4);
//    RuleScheme rs2;
//    rs2.add({2, 3}, "1N3");
//    TRS small = cd2.init_by_scheme(rs2);
//
//    TRS tranferred_trs = cd1.transfer_trs(large, small, {2, 3, 4, 5});
//    print_trs(tranferred_trs);
//    print_trs(large);

//    trs = cd.assign_by_index(trs, 1, "3N1");
//    std::cout << cd.condorcet_domain(trs).size() << std::endl;
//
//    TRS trs1 = cd.assign_by_index(trs, 1, "3N1");
//
//    TRS trs2 = cd.assign(trs1, {1, 3, 5}, "1N3");
//    TRS trs3 = cd.assign(trs2, {2, 3, 4}, "1N3");
//    print_trs(trs3);
//    Triplet triplet = cd.dynamic_triplet_ordering(trs3);
//    std::cout << triplet[0] << triplet[1] << triplet[2];

    return 0;
}


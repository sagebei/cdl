#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    RuleScheme rs;
    rs.add({2, 3, 4}, "1N3");
    TRS trs = cd.init_by_scheme(rs);

    TRS trs1 = cd.assign(trs, {1, 2, 5}, "3N1");
    TRS trs2 = cd.assign(trs1, {1, 3, 5}, "1N3");
    TRS trs3 = cd.assign(trs2, {2, 3, 4}, "1N3");
    print_trs(trs3);
    Triplet triplet = cd.dynamic_triplet_ordering(trs3);
    std::cout << triplet[0] << triplet[1] << triplet[2];

    return 0;
}


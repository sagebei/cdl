#include <iostream>
#include "condorcet_domain.h"


int main()
{
    int n = 15;
    TRS trs = initialize(n, true);
    change_rule(trs, 3, 0);
    change_rule(trs, 4, 1);
    change_rule(trs, 5, 2);
    change_rule(trs, 6, 3);
//    print_tr(trs);
//    TRS fetched_trs = fetch_triplet_rules(trs, 5);
//    print_tr(fetched_trs);

//    std::cout << triplet_rules.size() << std::endl;
    unsigned long size = condorcet_domain(trs, n);
    std::cout << "the size of the CD: " << size;
    return 0;
}


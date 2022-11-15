#include <iostream>
#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
//    RuleScheme rs;
//    rs.add({1, 2, 4}, "1N3");
//    rs.add({3, 5}, "2N1");
//
//    TRS trs = cd.initialize_by_scheme(rs);
//    cd.print_trs(trs);
    auto trs = cd.initialize(false);
    auto domain = cd.condorcet_domain(trs);
//    cd.print_cd(domain);
//    std::size_t seed = cd.hash_cd(domain);
    CDS bros = cd.cd_brothers(domain);
    for (const CD& bro: bros)
    {
        cd.print_cd(bro);
//        std::cout << cd.hash_cd(bro) << std::endl;
        std::cout << std::endl;

    }
//    std::cout << seed << std::endl;
    return 0;
}


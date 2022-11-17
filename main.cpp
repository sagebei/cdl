#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    RuleScheme rs;
    rs.add({2, 3}, "3N1");
    rs.add({4}, "1N3");

    TRS trs = cd.initialize_by_scheme(rs);
//    cd.print_trs(trs);
//    TRS trs = cd.initialize();
    CD domain = cd.condorcet_domain(trs);
    cd.print_cd(domain);
//    cd.sort_cd(domain);
//    cd.print_cd(domain);
    TRS all_trs = cd.cd_to_trs(domain);
    cd.print_trs(all_trs);
//    std::cout << all_trs.size() << std::endl;
//    std::cout << domain.size() << std::endl;
//    cd.print_cd(domain);
//    std::cout << cd.hash_cd(domain) << std::endl;
//    CDS bros = cd.cd_brothers(domain);
//    for (const CD& bro: bros)
//    {
//        std::cout << cd.hash_cd(bro) << std::endl;
//        cd.print_cd(bro);
//    }
//    std::cout << bros.size() << std::endl;
    return 0;
}


#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(8);
    std::cout << cd.num_triplets << std::endl;
//    RuleScheme rs;
//    rs.add({2}, "3N1");
//    rs.add({3}, "1N3");
//
//    TRS trs = cd.init_by_scheme(rs);
//    TRS trs = cd.init();
//
//    cd.assign_rule(trs, 0, "3N1");
//    cd.assign_rule(trs, 1, "3N1");
//    cd.assign_rule(trs, 2, "3N1");
//    cd.assign_rule(trs, 3, "3N1");
//    cd.print_trs(trs);

//    CD domain = cd.condorcet_domain(trs);
//    cd.print_cd(domain);
//    cd.sort_cd(domain);
//    cd.print_cd(domain);
//    TRS all_trs = cd.cd_to_trs(domain);
//    cd.print_trs(all_trs);
//    std::cout << all_trs.size() << std::endl;
//    std::cout << domain.size() << std::endl;
//    cd.print_cd(domain);
//    std::cout << cd.hash_cd(domain) << std::endl;
//    CDS bros = cd.cd_brothers(domain);
//    for (CD& bro: bros)
//    {
////        std::cout << cd.hash_cd(bro) << std::endl;
//        cd.print_cd(bro);
//    }
//    std::cout << cd.hash_cd_brothers(bros) << std::endl;
    return 0;
}


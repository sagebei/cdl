#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.init();

    cd.assign(trs, {1, 2, 5}, "3N1");
    print_trs(trs);
    std::vector<std::size_t> sizes = cd.evaluate_rules_on_triplet(trs, {1, 2, 3});
    for (std::size_t s: sizes)
        std::cout << s << " ";

//    cd.assign(trs, {1, 3, 5}, "3N1");
//    cd.assign(trs, {1, 2, 3}, "3N1");
//    cd.assign(trs, {1, 4, 5}, "3N1");
//    cd.print_trs(trs);
//    auto unassigned = cd.unassigned_triplets(trs);
//    for (auto& u: unassigned)
//    {
//        for (int i: u)
//            std::cout << i;
//       std::cout << std::endl;
//    }
//    std::cout << cd.num_triplets << std::endl;
//    RuleScheme rs;
//    rs.add({2}, "3N1");
//    rs.add({3, 4}, "1N3");
//
//    TRS trs = cd.init_by_scheme(rs);
//    cd.print_trs(trs);
//    TRS trs = cd.init();
//    std::cout << cd.condorcet_domain(trs).size() << std::endl;
//    auto cd_sizes = cd.subset_cd_sizes(trs, 4);
//    for (auto size: cd_sizes)
//        std::cout << size << std::endl;
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
//        std::cout << cd.hash_cd(bro) << std::endl;
//        cd.print_cd(bro);
//    }
//    std::cout << cd.hash_cd_brothers(bros) << std::endl;
    return 0;
}


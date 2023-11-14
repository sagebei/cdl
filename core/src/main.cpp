#include "condorcet_domain.h"

//std::string alternating_scheme(const Triple& triple)
//{
//    if ((triple[1] % 2) == 0)
//        return "2N1";
//    else
//        return "2N3";
//}
//
//int main()
//{
//    CondorcetDomain cd(12);
//    auto trs = cd.init_trs_by_scheme(alternating_scheme);
//    std::cout << cd.size(trs) << std::endl;
//    std::cout << cd.domain(trs).size() << std::endl;
//    std::cout << (cd.size(trs) == cd.domain(trs).size()) << std::endl;
//
//    CD domain = cd.domain(trs);
//    CDS domains{};
//    domains.push_back(domain);
//    domains.push_back(domain);
//
//    CDS new_cds = cd.non_isomorphic_domains(domains);
//    return 0;
//}

//#include "forbidden_permutation.h"
//
//Laws alternating_scheme(const Tuple& tuple)
//{
//    if ((tuple[1] % 2) == 0)
//        return {{2, 1, 3}, {2, 3, 1}};
//    else
//        return {{1, 3, 2}, {3, 1, 2}};
//}
//
//int main()
//{
//    ForbiddenPermutation fp(12, 3); // n=12, k=3
//    TLS tls = fp.init_tls_by_scheme(alternating_scheme);
//
//    std::cout << fp.domain(tls).size() << std::endl;
//    std::cout << fp.size(tls) << std::endl;
//    print_tls(tls);
//
//    return 0;
//}

//#include "forbidden_permutation.h"
//
//int main()
//{
//    for (int n = 4; n < 13; n ++)
//    {
//        ForbiddenPermutation fp(n, 4); // n=12, k=3
//        TLS tls = fp.init_tls();
//
//        for (int i = 0; i < tls.size(); i ++)
//        {
//            tls = fp.assign_laws_by_index(tls, i, {{1, 3, 4, 2}});
//        }
//
//        std::cout << fp.domain(tls).size() << std::endl;
//        std::cout << fp.size(tls) << std::endl;
//    }
//
//    return 0;
//}

//int main()
//{
//    CondorcetDomain cd(5);
//    auto trs = cd.init_trs();
//    trs = cd.assign_rule_by_index(trs, 0, "1N3");
//    trs = cd.assign_rule_by_index(trs, 1, "2N3");
//    trs = cd.assign_rule_by_index(trs, 2, "2N3");
//    trs = cd.assign_rule_by_index(trs, 3, "2N3");
//    trs = cd.assign_rule_by_index(trs, 4, "2N3");
//    trs = cd.assign_rule_by_index(trs, 5, "2N3");
//    trs = cd.assign_rule_by_index(trs, 6, "2N3");
//    trs = cd.assign_rule_by_index(trs, 7, "1N3");
//    trs = cd.assign_rule_by_index(trs, 8, "1N3");
//    trs = cd.assign_rule_by_index(trs, 9, "1N3");
//
//
//    print_trs(trs);
//    bool result = cd.is_trs_isomorphic(trs, {1, 3, 5}, {"1N3", "2N3"});
//    std::cout<< result << std::endl;
//    return 0;
//}

int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.init_trs();
//    TRS trs = cd.init_trs_by_scheme(Fishburn_scheme);
    CD domain = cd.domain(trs);
    CD sub_domain = cd.domain_on_triple(domain, {1, 2, 3});
    std::cout << sub_domain.size() << std::endl;
    return 0;
}

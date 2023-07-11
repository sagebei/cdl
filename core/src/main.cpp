//#include "condorcet_domain.h"
//
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
//    CondorcetDomain cd(8);
//    auto trs = cd.init_trs_by_scheme(alternating_scheme);
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

#include "forbidden_permutation.h"

int main()
{
    for (int n = 4; n < 13; n ++)
    {
        ForbiddenPermutation fp(n, 4); // n=12, k=3
        TLS tls = fp.init_tls();

        for (int i = 0; i < tls.size(); i ++)
        {
            tls = fp.assign_laws_by_index(tls, i, {{1, 3, 4, 2}});
        }

        std::cout << fp.domain(tls).size() << std::endl;
        std::cout << fp.size(tls) << std::endl;
    }

    return 0;
}


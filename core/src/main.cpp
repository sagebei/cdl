#include "condorcet_domain.h"
#include "forbidden_permutation.h"

int main()
{
    ForbiddenPermutation fp(4);
    TLS tls = fp.init_tls();
    std::vector<std::string> l1{"213", "231"};
    std::vector<std::string> l2{"132", "312"};
    TLS mew_trs = fp.init_tls();
    for (auto& tl : tls)
    {
        if (tl.triplet[1] % 2 == 0)
            mew_trs = fp.assign_laws(mew_trs, tl.triplet, l1); // 2N1
        else
            mew_trs = fp.assign_laws(mew_trs, tl.triplet, l2); // 2N3
    }


    std::cout << fp.size(mew_trs) << std::endl;
    std::cout << fp.domain(mew_trs).size() << std::endl;
//    CondorcetDomain cd(13);
//    std::cout << cd.n << std::endl;
//    TRS trs = cd.init_by_scheme(Fishburn_scheme);
//    std::cout << cd.size(trs) << std::endl;
//    std::cout << cd.condorcet_domain(trs).size() << std::endl;
//    CD domain = cd.condorcet_domain(trs);
//    CDS domains{};
//    domains.push_back(domain);
//    domains.push_back(domain);
//
//    CDS new_cds = cd.isomorphic_cds(domains);
    return 0;
}


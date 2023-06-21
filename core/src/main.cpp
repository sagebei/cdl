#include "condorcet_domain.h"
#include "wrapper.h"

int main()
{
    CondorcetDomain cd(13);
//    std::cout << cd.n << std::endl;
    TRS trs = cd.init_by_scheme(Fishburn_scheme);
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


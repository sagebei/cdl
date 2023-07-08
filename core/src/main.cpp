#include "condorcet_domain.h"

std::string alternating_scheme(const Triple& triple)
{
    if ((triple[1] % 2) == 0)
        return "2N1";
    else
        return "2N3";
}

int main()
{
    CondorcetDomain cd(6);
    auto trs = cd.init_trs_by_scheme(alternating_scheme);
    std::cout << (cd.size(trs) == cd.domain(trs).size()) << std::endl;

    CD domain = cd.domain(trs);
    CDS domains{};
    domains.push_back(domain);
    domains.push_back(domain);

    CDS new_cds = cd.non_isomorphic_domains(domains);
    return 0;
}


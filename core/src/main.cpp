#include "condorcet_domain.h"
#include "wrapper.h"

int main()
{
    CondorcetDomain cd(6);
    TRS trs = cd.init_by_scheme(Fishburn_scheme);
    CD domain = cd.condorcet_domain(trs);
    CDS domains{};
    domains.push_back(domain);
    domains.push_back(domain);

    CDS new_cds = cd.isomorphic_cds(domains);
    return 0;
}


#include "condorcet_domain.h"


int main()
{
    CondorcetDomain cd(5);
    TRS trs = cd.init();

    cd.assign(trs, {1, 2, 5}, "3N1");
    std::cout << cd.condorcet_domain(trs).size() << std::endl;
    print_trs(trs);
    std::vector<std::size_t> sizes = cd.evaluate_rules_on_triplet(trs, {1, 2, 3});
    for (std::size_t s: sizes)
        std::cout << s << " ";

    return 0;
}


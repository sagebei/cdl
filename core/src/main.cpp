#include "condorcet_domain.h"
#include "wrapper.h"

int main()
{
    CondorcetDomain cd(6);
    TRSWrapper wrapper(cd);
    RuleScheme rs;
    rs.add({2, 4}, "1N3");
    rs.add({5}, "3N1");
    TRS trs = cd.init_by_scheme(rs);
    wrapper.dynamic_triplet_ordering(trs);

//    RuleScheme rs;
//    rs.add({2, 3}, "1N3");
//    rs.add({4, 5}, "3N1");
//    TRS trs = cd.init_by_scheme(rs);
//
//    auto states = cd.subsets_state(trs);
//    for (const auto& state: states)
//    {
//        for(const int& s: state)
//            std::cout<< s << " ";
//        std::cout << std::endl;
//    }
//    std::cout << states.size() << std::endl;
//
//    auto result = cd.subset_cd_sizes(trs);

    return 0;
}


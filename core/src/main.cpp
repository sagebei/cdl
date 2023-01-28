#include "condorcet_domain.h"
#include "wrapper.h"
#include "utils.h"

int main()
{
    CondorcetDomain cd(5);
//    TRS trs = cd.init_random();
//    print_trs(trs);
//    std::cout << std::endl;
//
//    print_trs(cd.shuffle_trs(trs, 91));


//    TRS empty_trs = cd.init_empty(false);
//    TRS trs = cd.init_random();
//    print_trs(trs);
//    empty_trs = cd.transfer_trs(trs, empty_trs);
//    print_trs(empty_trs);
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    std::vector<std::vector<int>> com = combinations(v, 5);
    for (const auto& co : com)
    {
        for (const auto & c : co)
        {
            std::cout << c;

        }
        std::cout << std::endl;
    }

    return 0;
}


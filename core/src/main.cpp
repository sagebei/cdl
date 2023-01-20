#include "condorcet_domain.h"
#include "wrapper.h"

int main()
{
    CondorcetDomain cd(5);
//    TRS trs = cd.init_random();
//    print_trs(trs);
//    std::cout << std::endl;
//
//    print_trs(cd.shuffle_trs(trs, 91));


    TRS empty_trs = cd.init_empty(false);
    TRS trs = cd.init_random();
    print_trs(trs);
    empty_trs = cd.transfer_trs(trs, empty_trs);
    print_trs(empty_trs);

    return 0;
}


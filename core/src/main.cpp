#include "condorcet_domain.h"

int main()
{
    CondorcetDomain cd(5);
    TRS trs;
    std::vector<Triple> triples = {{1, 4, 5}, {1, 3, 4}, {2, 3, 5}, {1, 2, 4}, {2, 4, 5},{1, 3, 5},{1, 2, 5},{2, 3, 4},{1, 2, 3},{3, 4, 5}};
    for (Triple t : triples)
    {
        TripleRule tr{};
        tr.triple = t;
        tr.rule_id = 0;
        trs.push_back(tr);

    }

    trs = cd.assign_rule_any_ordering(trs, {1, 4, 5}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {1, 3, 4}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {2, 3, 5}, "1N3");
    trs = cd.assign_rule_any_ordering(trs, {1, 2, 4}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {2, 4, 5}, "1N3");
    trs = cd.assign_rule_any_ordering(trs, {1, 3, 5}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {1, 2, 5}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {2, 3, 4}, "2N3");
    trs = cd.assign_rule_any_ordering(trs, {1, 2, 3}, "");
    trs = cd.assign_rule_any_ordering(trs, {3, 4, 5}, "");

    print_trs(trs);
    bool result = cd.is_trs_isomorphic(trs, {"1N3", "2N3"});
    std::cout<< result << std::endl;
    return 0;
}


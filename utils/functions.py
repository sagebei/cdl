from dataclasses import dataclass
from cdl import *
from itertools import groupby, product


@dataclass
class TDS:
    trs: list
    domain: list
    size: int


# Verify if a domain is maximal
def maximality_check(cd, tds):
    hidden_trs = cd.domain_to_trs(tds.domain)
    trs_tuple = []
    for tr in hidden_trs:
        trs_tuple.append((tr.triple, tr.rule_id))
    groups = []
    for _, group in groupby(trs_tuple, lambda x: x[0]):
        groups.append(list(group))

    for trs_tuple in product(*groups):
        trs = cd.init_trs_colex()
        for t, r in trs_tuple:
            trs = cd.assign_id_any_ordering(trs, t, r)
        if cd.size(trs) > tds.size:
            return False

    return True










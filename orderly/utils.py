import pickle
from collections import Counter
import os
from collections import OrderedDict
import random
from dataclasses import dataclass
from cdl import *


@dataclass
class TDS:
    trs: list
    domain: list
    size: int


class Search:
    def __init__(self, cd, rules, result_path):
        self.cd = cd
        self.rules = rules
        self.result_path = result_path
        self.folder_path = result_path / f"{cd.n}"

    def next_triple(self, trs):
        unassigned = self.cd.unassigned_triples(trs)
        if len(unassigned) == 0:
            return [0, 0, 0]

        return unassigned[0]

    def check_tds(self, tds, cutoff_abundance, cutoff_size):
        if not self.cd.is_domain_copious(tds.trs, tds.domain):
            return False

        if self.cd.is_trs_isomorphic(tds.trs, self.rules):
            return False

        return True

    def build_new_tds(self, tds, triple, rule, cutoff_size):
        new_trs = self.cd.assign_rule_any_ordering(tds.trs, triple, rule)

        tr = TripleRule()
        tr.triple = triple
        tr.rule_id = self.cd.rule_id[rule]

        new_domain = self.cd.filter_domain(tds.domain, tr)
        new_size = len(new_domain)

        new_tds = TDS(new_trs, new_domain, new_size)

        return new_tds

    def expand_tds(self, tds, cutoff_abundance=16, cutoff_size=100):
        triple = self.next_triple(tds.trs)
        new_tds_list = []
        for rule in self.rules:
            new_tds = self.build_new_tds(tds, triple, rule, cutoff_size)

            if self.check_tds(new_tds, cutoff_abundance, cutoff_size):
                new_tds_list.append(new_tds)

        return new_tds_list

    def save(self,
             tds_list,
             sub_folder_name,
             filename):

        sub_folder_path = self.folder_path / sub_folder_name
        sub_folder_path.mkdir(parents=True, exist_ok=True)
        filepath = sub_folder_path / filename
        with filepath.open("wb") as f:
            trs_size_list = []

            for tds in tds_list:
                trs_size_list.append((self.cd.trs_to_state(tds.trs), tds.size))
            pickle.dump(trs_size_list, f)

    def load(self, filepath):
        tds_list = []
        with filepath.open("rb") as f:
            for state, _ in pickle.load(f):
                trs = self.cd.state_to_trs(self.cd.init_trs_colex(), state)
                domain = self.cd.domain(trs)
                # print(len(domain))
                tds_list.append(TDS(trs, domain, len(domain)))

        return tds_list

    def get_size_counter(self):
        sizes = []
        trs_size_list = []

        folder_path = self.folder_path / f"{self.cd.num_triples}_{self.cd.num_triples}"
        for filepath in folder_path.iterdir():
            with filepath.open("rb") as f:
                for tds in pickle.load(f):
                    sizes.append(tds.size)
                    trs_size_list.append((tds.trs, tds.size))

        result = Counter(sizes)
        result = OrderedDict(sorted(result.items(), key=lambda t: t[0]))

        with open(f"{self.folder_path}/trs_size_list.pkl", "wb") as f:
            pickle.dump(trs_size_list, f)

        return result


def split_into_chunks(lst, n):
    """Split a list into n chunks (as evenly sized as possible)."""
    k, m = divmod(len(lst), n)
    l = [lst[i*k + min(i, m):(i+1)*k + min(i+1, m)] for i in range(n)]
    return [i for i in l if len(i) > 0]


if __name__ == "__main__":
    results = split_into_chunks([1, 2, 3], 10)
    print(results)
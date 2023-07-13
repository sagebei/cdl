from cdl import *
import pickle
from copy import deepcopy
from itertools import combinations, product


class Generator:
    def __init__(self):
        self.cd = CondorcetDomain(n=5)
        self.trs = self.cd.init_trs()
        for tr in self.trs:
            self.trs = self.cd.assign_rule(self.trs, tr.triple, "3N1")
        self.triples = [tr.triple for tr in self.trs]
        self.dataset_dict = {}
        self.rules = ["2N3", "2N1"]

    def generate_full(self):
        trs = deepcopy(self.trs)
        rule_list = [self.rules for _ in range(self.cd.num_triples)]
        for rules in product(*rule_list):
            for i, rule in enumerate(rules):
                trs = self.cd.assign_rule_by_index(trs, i, rule)
                state = self.cd.trs_to_state(trs)
                self.dataset_dict[tuple(state)] = self.cd.size(trs)

    def generate_all(self):
        for i in range(1, self.cd.num_triples + 1):
            print(i)
            for triples in combinations(self.triples, i):
                trs = deepcopy(self.trs)
                for triple in triples:
                    trs = self.cd.assign_rule(trs, triple, "")

                self.generate_on_trs(trs)

        with open("database_5_2_rules.pkl", "wb") as f:
            pickle.dump(self.dataset_dict, f)

    def generate_on_trs(self, trs):
        trs = deepcopy(trs)
        assigned = self.cd.assigned_triples(trs)
        unassigned = self.cd.unassigned_triples(trs)
        num_assigned = len(assigned)

        rule_list = [self.rules for _ in range(num_assigned)]

        for rp in product(*rule_list):
            for i, rule in enumerate(rp):
                trs = self.cd.assign_rule(trs, assigned[i], rule)

            largest_size = -1
            for u in unassigned:
                for r in self.rules:
                    u_trs = self.cd.assign_rule(trs, u, r)
                    state = self.cd.trs_to_state(u_trs)
                    size = self.dataset_dict[tuple(state)]
                    if size > largest_size:
                        largest_size = size

            state = self.cd.trs_to_state(trs)
            self.dataset_dict[tuple(state)] = largest_size


if __name__ == "__main__":
    from collections import Counter

    gen = Generator()

    gen.generate_full()
    gen.generate_all()

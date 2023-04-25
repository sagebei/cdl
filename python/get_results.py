from cdl import *
from utils import StaticFeature5, Search
import argparse
import numpy as np
import os


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules):
        super().__init__(cd, rules)
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules))
        self.rules = rules

    def static_search(self,
                      trs,
                      cutoff=16,
                      n_complete_triplet=20,
                      n_cores=10):

        folder_name = f"_".join(self.rules)
        num_unassigned = len(self.cd.unassigned_triplets(trs))

        trs_score_list = self.expand_trs(trs)
        trs_list = [trs for (trs, score) in trs_score_list]

        for n_iter in range(2, n_complete_triplet+1):
            next_trs_value_list = []

            for trs in trs_list:
                trs_value_list = self.expand_trs(trs, cutoff)
                next_trs_value_list.extend(trs_value_list)

            trs_list.clear()
            trs_list = [trs for (trs, score) in next_trs_value_list]

        split_trs_list = np.array_split(np.array(trs_list, dtype=object), n_cores)
        for i, t in enumerate(split_trs_list):
            self.save_trs_list(trs_list=t.tolist(),
                               core_id=1,
                               folder_name=folder_name,
                               sub_folder_name=f"{n_complete_triplet}_{num_unassigned}",
                               filename=f"{i+1}.pkl",
                               remove=False)


parser = argparse.ArgumentParser(description="complete search for the first n triplet",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int)
parser.add_argument("-cutoff", type=int)
parser.add_argument("-rules", nargs="*", type=str)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = Search(cd, rules=config['rules'])
folder_name = f"{config['cutoff']}_" + f"_".join(config['rules'])
results = es.get_size_counter(folder_name)
print(results)

if not os.path.exists("results"):
    os.makedirs("results")

with open(f"./results/{folder_name}.txt", "w") as f:
    f.write(str(results))

# python get_results.py -n 6 -cutoff 16 -rules "2N3" "2N1"


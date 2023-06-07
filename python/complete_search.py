from cdl import *
from utils import StaticFeature5, Search
from tools import init_rules, flip_exceptions
import argparse
import numpy as np
import random


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules), lib_path=lib_path)
        self.rules = rules

    def static_search(self,
                      trs,
                      cutoff=16,
                      threshold=0,
                      top_n=1000,
                      n_complete=20,
                      n_cores=10,
                      n_chunks=1000,
                      shuffle=False):

        num_assigned = len(self.cd.assigned_triplets(trs))

        folder_name = f"{cutoff}_{threshold}_{top_n}_{n_cores}_{n_chunks}_{num_assigned+n_complete}_{shuffle}_" + f"_".join(self.rules)
        self.folder_path += folder_name

        trs_score_list = self.expand_trs(trs)

        for n_iter in range(num_assigned+2, num_assigned+n_complete+1):
            next_trs_score_list = []

            for trs, _ in trs_score_list:
                trs_value_list = self.expand_trs(trs, cutoff, threshold)
                next_trs_score_list.extend(trs_value_list)

            trs_score_list.clear()
            trs_score_list = next_trs_score_list

        if shuffle:
            random.shuffle(trs_score_list)

        # trs_score_list.sort(key=lambda trs_score: trs_score[1])
        print(len(trs_score_list))
        split_trs_score_list = np.array_split(np.array(trs_score_list, dtype=object),
                                              min(n_chunks, len(trs_score_list)))
        for i, t in enumerate(split_trs_score_list):
            self.save_trs_score_list(trs_list=t.tolist(),
                                     sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triplets}",
                                     filename=f"{i+1}.pkl")


parser = argparse.ArgumentParser(description="complete search for the first n triplet",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=6)
parser.add_argument("-rules", nargs="*", type=str, default=["1N3", "2N3"])
parser.add_argument("-cutoff", type=int, default=14)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-top_n", type=int, default=100000)
parser.add_argument("-n_complete", type=int, default=20)
parser.add_argument("-n_cores", type=int, default=10)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="../")
parser.add_argument("-result_path", type=str, default="./results")
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
trs = cd.init_trs()

# low_exceptions = [(1,3),(1,4),(3,4),(2,5),(2,6),(5,6)]
# high_exceptions = flip_exceptions(cd, low_exceptions)
# high_exceptions = [(10,12),(9,12),(9,10),(8,11),(7,11),(7,8)]
# trs = init_rules(cd,
#                  trs,
#                  low_exceptions,
#                  high_exceptions)

es.static_search(trs,
                 cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 top_n=config['top_n'],
                 n_complete=config['n_complete'],
                 n_cores=config['n_cores'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'])




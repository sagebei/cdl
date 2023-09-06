from cdl import *
from utils import Search
import argparse
import numpy as np
import random


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)

    def static_search(self,
                      trs,
                      cutoff=16,
                      threshold=0,
                      n_complete=20,
                      n_chunks=1000,
                      shuffle=False):

        if n_complete == -1:
            n_complete = cd.num_triples

        num_assigned = len(self.cd.assigned_triples(trs))

        folder_name = f"{cutoff}_{threshold}_{n_chunks}_{num_assigned+n_complete}_{shuffle}_" + f"_".join(self.rules)
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

        print(len(trs_score_list))
        split_trs_score_list = np.array_split(np.array(trs_score_list, dtype=object),
                                              min(n_chunks, len(trs_score_list)))
        for i, t in enumerate(split_trs_score_list):
            self.save_trs_score_list(trs_list=t.tolist(),
                                     sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triples}",
                                     filename=f"{i+1}.pkl")


parser = argparse.ArgumentParser(description="complete search for the first n triple",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=9)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "2N3"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-n_complete", type=int, default=5)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="E:/cdl")
parser.add_argument("-result_path", type=str, default="./results")
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
trs = cd.init_trs()

es.static_search(trs,
                 cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 n_complete=config['n_complete'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'])




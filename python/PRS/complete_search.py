from cdl import *
from utils import Search
from tools import init_rules, flip_exceptions
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
                      top_n=1000,
                      n_complete=20,
                      n_chunks=1000,
                      shuffle=False):

        if n_complete == -1:
            n_complete = cd.num_triples

        num_assigned = len(self.cd.assigned_triples(trs))

        folder_name = f"{cutoff}_{threshold}_{top_n}_{n_chunks}_{num_assigned+n_complete}_{shuffle}_" + f"_".join(self.rules)
        self.folder_path += folder_name

        trs_score_list = self.expand_trs(trs)

        for n_iter in range(num_assigned+2, num_assigned+n_complete+1):
            next_trs_score_list = []

            for trs, _ in trs_score_list:
                trs_value_list = self.expand_trs(trs, cutoff, threshold)
                next_trs_score_list.extend(trs_value_list)

            trs_score_list.clear()
            trs_score_list = next_trs_score_list

            if n_complete == cd.num_triples and top_n != -1:
                trs_score_list.sort(key=lambda trs_score: trs_score[1])
                trs_score_list = trs_score_list[-top_n:]

        if shuffle:
            random.shuffle(trs_score_list)

        if len(trs_score_list) > 0:
            print(len(trs_score_list))
            split_trs_score_list = np.array_split(np.array(trs_score_list, dtype=object),
                                                  min(n_chunks, len(trs_score_list)))
            for i, t in enumerate(split_trs_score_list):
                self.save_trs_score_list(trs_list=t.tolist(),
                                         sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triples}",
                                         filename=f"{i+1}.pkl")
        else:
            print("No such domains")


parser = argparse.ArgumentParser(description="complete search for the first n triple",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=6)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "1N3"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-top_n", type=int, default=1000)
parser.add_argument("-n_complete", type=int, default=-1)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="/Users/bei/CLionProjects/cdl/")
parser.add_argument("-result_path", type=str, default="./results")
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
trs = cd.init_trs()

# low_exceptions = [(1,3),(2,4)]
# high_exceptions = flip_exceptions(cd, low_exceptions)
# high_exceptions = [(5,7),(6,8)]
# trs = init_rules(cd,
#                  trs,
#                  low_exceptions,
#                  high_exceptions)

es.static_search(trs,
                 cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 top_n=config['top_n'],
                 n_complete=config['n_complete'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'])




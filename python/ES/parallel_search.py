import os
from cdl import *
from utils import Search
from tools import get_unprocessed_fileid
import argparse
import sys
sys.setrecursionlimit(5000)


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)
        self.chunk_id = 1

    def fill_trs(self,
                 trs,
                 cutoff=16,
                 threshold=0,
                 full_trs_score_list=[],
                 chunk_size=10,
                 file_id=0):

        triple = self.cd.next_unassigned_triple(trs)
        if triple == [0, 0, 0]:
            score = self.sf.score_function(trs, cutoff, threshold)
            if score > -1:
                full_trs_score_list.append((trs, score))
                if len(full_trs_score_list) >= chunk_size:
                    self.save_trs_score_list(full_trs_score_list,
                                             f"{self.cd.num_triples}_{self.cd.num_triples}",
                                             f"{file_id}_{self.chunk_id}.pkl")
                    full_trs_score_list.clear()
                    self.chunk_id += 1
        else:
            for rule in self.rules:
                new_trs = self.cd.assign_rule(trs, triple, rule)
                score = self.sf.score_function(new_trs, cutoff, threshold)

                if score == -1:
                    continue
                else:
                    self.fill_trs(new_trs,
                                  cutoff,
                                  threshold,
                                  full_trs_score_list,
                                  chunk_size,
                                  file_id)

    def static_search(self,
                      cutoff=16,
                      threshold=0,
                      n_complete=5,
                      n_chunks=1000,
                      shuffle=False,
                      chunk_size=100000):

        folder_name = f"{cutoff}_{threshold}_{n_chunks}_{n_complete}_{shuffle}_" + f"_".join(self.rules)

        self.folder_path += folder_name
        sub_folder_path = f"{self.folder_path}/{n_complete}_{self.cd.num_triples}/"

        file_id = get_unprocessed_fileid(sub_folder_path)
        while file_id is not None:
            try:
                os.rename(sub_folder_path+f"{file_id}.pkl",
                          sub_folder_path+f"{file_id}.processing")

                trs_score_list = self.load_trs_score_list(f"{n_complete}_{self.cd.num_triples}",
                                                          f"{file_id}.processing")

                full_trs_score_list = []
                self.chunk_id = 1
                for trs, _ in trs_score_list:
                    self.fill_trs(trs,
                                  cutoff,
                                  threshold,
                                  full_trs_score_list,
                                  chunk_size,
                                  file_id)

                if len(full_trs_score_list) > 0:
                    self.save_trs_score_list(full_trs_score_list,
                                             f"{self.cd.num_triples}_{self.cd.num_triples}",
                                             f"{file_id}_{0}.pkl")

                os.remove(sub_folder_path+f"{file_id}.processing")

            except FileNotFoundError as e:
                print(e)

            file_id = get_unprocessed_fileid(sub_folder_path)


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=7)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "2N3"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-n_complete", type=int, default=5)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="/Users/bei/CLionProjects/cdl")
parser.add_argument("-result_path", type=str, default="./results")
parser.add_argument("-chunk_size", type=int, default=10)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
es.static_search(cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 n_complete=config['n_complete'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'],
                 chunk_size=config["chunk_size"])


# python parallel_search.py -n 6 -cutoff 16 -threshold 0 -top_n 1000 -rules "2N3" "2N1"  -triple_id 6 -core_id 2



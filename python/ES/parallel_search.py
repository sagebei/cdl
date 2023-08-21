import os
from cdl import *
from utils import Search
from tools import get_unprocessed_fileid
import argparse
from itertools import product


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)

    def static_search(self,
                      cutoff=16,
                      threshold=0,
                      top_n=1000,
                      n_complete=5,
                      n_chunks=1000,
                      shuffle=False):

        folder_name = f"{cutoff}_{threshold}_{top_n}_{n_chunks}_{n_complete}_{shuffle}_" + f"_".join(self.rules)

        self.folder_path += folder_name
        sub_folder_path = f"{self.folder_path}/{n_complete}_{self.cd.num_triples}/"

        file_id = get_unprocessed_fileid(sub_folder_path)
        while file_id is not None:
            try:
                os.rename(sub_folder_path+f"{file_id}.pkl",
                          sub_folder_path+f"{file_id}.processing")

                trs_score_list = self.load_trs_score_list(f"{n_complete}_{self.cd.num_triples}",
                                                          f"{file_id}.processing")

                full_trs_list = []
                rules_list = [self.rules for _ in range(len(self.cd.unassigned_triples(trs_score_list[0][0])))]
                for trs, _ in trs_score_list:
                    rules_iter = product(*rules_list)
                    for rules in rules_iter:
                        full_trs, score = self.fill_trs(trs, rules, cutoff, threshold)
                        if full_trs is None:
                            continue
                        full_trs_list.append((full_trs, score))

                self.save_trs_score_list(full_trs_list,
                                         f"{self.cd.num_triples}_{self.cd.num_triples}",
                                         f"{file_id}.pkl")
                os.remove(sub_folder_path+f"{file_id}.processing")

            except FileNotFoundError as e:
                print(e)

            file_id = get_unprocessed_fileid(sub_folder_path)


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=6)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "2N3"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-top_n", type=int, default=1000)
parser.add_argument("-n_complete", type=int, default=10)
parser.add_argument("-n_chunks", type=int, default=10)

parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="/Users/bei/CLionProjects/cdl/")
parser.add_argument("-result_path", type=str, default="./results")
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
es.static_search(cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 top_n=config['top_n'],
                 n_complete=config['n_complete'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'])


# python parallel_search.py -n 6 -cutoff 16 -threshold 0 -top_n 1000 -rules "2N3" "2N1"  -triple_id 6 -core_id 2



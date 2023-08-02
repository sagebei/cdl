import os
from cdl import *
from utils import Search
from tools import get_unprocessed_fileid
import argparse


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

                for n_iter in range(n_complete+1, self.cd.num_triples+1):
                    next_trs_score_list = []

                    for trs, _ in trs_score_list:
                        trs_value_list = self.expand_trs(trs, cutoff, threshold)
                        next_trs_score_list.extend(trs_value_list)

                    trs_score_list.clear()

                    if top_n == -1:
                        trs_score_list = next_trs_score_list
                    else:
                        next_trs_score_list.sort(key=lambda trs_score: trs_score[1])
                        trs_score_list = next_trs_score_list[-top_n:]

                self.save_trs_score_list(trs_score_list,
                                         f"{self.cd.num_triples}_{self.cd.num_triples}",
                                         f"{file_id}.pkl")
                os.remove(sub_folder_path+f"{file_id}.processing")

            except FileNotFoundError as e:
                print(e)

            file_id = get_unprocessed_fileid(sub_folder_path)


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=6)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "2N3", "1N3", "3N1"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-top_n", type=int, default=10000)
parser.add_argument("-n_complete", type=int, default=10)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="../")
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



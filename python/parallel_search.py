import os
import random

from cdl import *
from utils import StaticFeature5, Search
import argparse
from tqdm import tqdm


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules), lib_path=lib_path)
        self.rules = rules

    def static_search(self,
                      cutoff=16,
                      threshold=0,
                      top_n=1000,
                      n_complete=5,
                      n_cores=16,
                      n_chunks=1000,
                      shuffle=False,
                      core_id=1):

        folder_name = f"{cutoff}_{threshold}_{top_n}_{n_cores}_{n_chunks}_{n_complete}_{shuffle}_" + f"_".join(self.rules)

        trs_score_list = self.load_trs_list(folder_name,
                                            f"{n_complete}_{self.cd.num_triplets}",
                                            f"{core_id}.pkl")

        for n_iter in range(n_complete+1, cd.num_triplets+1):
            print(f"{core_id}_{n_iter}")
            next_trs_score_list = []

            for trs, _ in trs_score_list:
                trs_value_list = self.expand_trs(trs, cutoff, threshold)
                next_trs_score_list.extend(trs_value_list)

            trs_score_list.clear()
            next_trs_score_list.sort(key=lambda trs_score: trs_score[1])
            trs_score_list = next_trs_score_list[-top_n:]

            self.save_trs_list(trs_score_list,
                               folder_name,
                               f"{n_iter}_{self.cd.num_triplets}",
                               f"{core_id}.pkl",
                               remove=True)

        subfolder_name = f"{self.result_path}/{self.cd.n}/{folder_name}/{n_complete}_{self.cd.num_triplets}/"
        unprocessed_filenames = os.listdir(subfolder_name)
        while len(unprocessed_filenames) > 0:
            filename = random.choice(unprocessed_filenames)
            file_id, file_extension = filename.split(".")
            file_id = int(file_id)

            if file_id > n_cores and file_extension == "pkl":
                os.rename(subfolder_name+filename,
                          subfolder_name+f"{file_id}.processing")

                trs_score_list = self.load_trs_list(folder_name,
                                                    f"{n_complete}_{self.cd.num_triplets}",
                                                    f"{file_id}.processing")

                for n_iter in range(n_complete+1, cd.num_triplets+1):
                    print(f"{file_id}_{n_iter}")
                    next_trs_score_list = []

                    for trs, _ in trs_score_list:
                        trs_value_list = self.expand_trs(trs, cutoff, threshold)
                        next_trs_score_list.extend(trs_value_list)

                    trs_score_list.clear()
                    next_trs_score_list.sort(key=lambda trs_score: trs_score[1])
                    trs_score_list = next_trs_score_list[-top_n:]

                    self.save_trs_list(trs_score_list,
                                       folder_name,
                                       f"{n_iter}_{self.cd.num_triplets}",
                                       filename,
                                       remove=True)

            unprocessed_filenames = os.listdir(folder_name + f"/{n_complete}_{self.cd.num_triplets}/")


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int)
parser.add_argument("-rules", nargs="*", type=str)
parser.add_argument("-cutoff", type=int)
parser.add_argument("-threshold", type=int)
parser.add_argument("-top_n", type=int)
parser.add_argument("-n_complete", type=int)
parser.add_argument("-n_cores", type=int)
parser.add_argument("-n_chunks", type=int)
parser.add_argument("-shuffle", type=bool)
parser.add_argument("-core_id", type=int)
parser.add_argument("-lib_path", type=str)
parser.add_argument("-result_path", type=str)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
es.static_search(cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 top_n=config['top_n'],
                 n_complete=config['n_complete'],
                 n_cores=config['n_cores'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'],
                 core_id=config['core_id'])


# python parallel_search.py -n 6 -cutoff 16 -threshold 0 -top_n 1000 -rules "2N3" "2N1"  -triplet_id 6 -core_id 2



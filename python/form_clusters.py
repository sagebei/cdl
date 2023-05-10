from cdl import *
from utils import StaticFeature5, Search
from tools import init_rules, flip_exceptions
import argparse
import numpy as np
import random
from sklearn.cluster import KMeans


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

        folder_name = f"{cutoff}_{threshold}_{top_n}_{n_cores}_{n_chunks}_{n_complete}_{shuffle}_" + f"_".join(self.rules)
        num_assigned = len(self.cd.assigned_triplets(trs))

        trs_score_list = self.expand_trs(trs)

        for n_iter in range(num_assigned+2, num_assigned+n_complete+1):
            next_trs_score_list = []

            for trs, _ in trs_score_list:
                trs_value_list = self.expand_trs(trs, cutoff, threshold)
                next_trs_score_list.extend(trs_value_list)

            trs_score_list.clear()
            trs_score_list = next_trs_score_list

        features = []
        for (trs, score) in trs_score_list:
            feature = self.sf.fetch_feature(trs)
            features.append(feature)

        clustered_trs_score_list = [[] for _ in range(n_chunks)]
        cluster_model = KMeans(n_clusters=n_chunks,
                               n_init=100,
                               max_iter=1000)
        labels = cluster_model.fit_predict(features)
        for ((trs, score), label) in zip(trs_score_list, labels):
            clustered_trs_score_list[label].append((trs, score))

        for i, t in enumerate(clustered_trs_score_list):
            self.save_trs_list(trs_list=t,
                               folder_name=folder_name,
                               sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triplets}",
                               filename=f"{i+1}.pkl",
                               remove=False)


parser = argparse.ArgumentParser(description="complete search for the first n triplet",
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
parser.add_argument("-lib_path", type=str)
parser.add_argument("-result_path", type=str)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
trs = cd.init_trs()

# low_exceptions = [(1, 3), (1, 4), (3, 4), (2, 5)]
# high_exceptions = flip_exceptions(cd, low_exceptions)
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


# python complete_search.py -n 6 -cutoff 16 threshold=0 -rules "2N3" "2N1" -n_complete 5 -n_cores 2


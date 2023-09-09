import os
import random
import platform
from cdl import *
from utils import Search
from tools import get_unprocessed_fileid
import argparse
import sys
import time
sys.setrecursionlimit(5000)


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, lib_path, result_path):
        super().__init__(cd, rules, lib_path, result_path)
        self.n_complete = 0

        self.chunk_id = 1
        self.split_id = 1
        self.triple_rule_dict = {}
        self.triple_index = {(0, 0, 0): -1}
        self.start_time = time.time()
        self.per_trs_time_limit = 0

    def set_triple_dict(self, trs):
        for idx, tr in enumerate(trs):
            self.triple_rule_dict[tuple(tr.triple)] = self.rules
            self.triple_index[tuple(tr.triple)] = idx

    def split_trs(self, trs, from_triple_idx, file_id):
        for cur_triple_idx in range(from_triple_idx, len(trs)):
            cur_triple = tuple(trs[cur_triple_idx].triple)
            cur_rule_id = trs[cur_triple_idx].rule_id
            if cur_rule_id == 0:
                cur_rule = self.rules[0]
            else:
                cur_rule = cd.rules[cur_rule_id-1]

            if cur_rule != self.rules[-1] and len(self.triple_rule_dict[cur_triple]) != 1:
                cur_rule_index = self.rules.index(cur_rule)

                self.triple_rule_dict[cur_triple] = [cur_rule]

                for next_rule in self.rules[cur_rule_index+1:]:
                    new_trs = self.cd.assign_rule_by_index(trs, cur_triple_idx, next_rule)
                    for i in range(cur_triple_idx+1, len(trs)):
                        new_trs = self.cd.assign_id_by_index(new_trs, i, 0)

                    self.save_trs_score_list([(new_trs, 0)],
                                             f"{self.n_complete}_{self.cd.num_triples}",
                                             f"{file_id}_{self.split_id}.pkl")
                    self.split_id += 1

                break

        self.start_time = time.time()

    def fill_trs(self,
                 trs,
                 cutoff=16,
                 threshold=0,
                 full_trs_score_list=[],
                 from_triple_idx=5,
                 chunk_size=10,
                 file_id=""):

        if len(file_id) < 250:
            if (time.time() - self.start_time) > self.per_trs_time_limit:
                self.split_trs(trs, from_triple_idx, file_id)

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
                if rule in self.triple_rule_dict[tuple(triple)]:
                    new_trs = self.cd.assign_rule(trs, triple, rule)
                    score = self.sf.score_function(new_trs, cutoff, threshold)

                    if score == -1:
                        continue
                    else:
                        self.fill_trs(new_trs,
                                      cutoff,
                                      threshold,
                                      full_trs_score_list,
                                      from_triple_idx,
                                      chunk_size,
                                      file_id)

    def static_search(self,
                      cutoff=16,
                      threshold=0,
                      n_complete=5,
                      n_chunks=1000,
                      shuffle=False,
                      per_trs_time_limit=60*60,
                      chunk_size=100000):
        self.n_complete = n_complete
        self.per_trs_time_limit = per_trs_time_limit * 60

        folder_name = f"{cutoff}_{threshold}_{n_chunks}_{n_complete}_{shuffle}_" + f"_".join(self.rules)

        self.folder_path += folder_name
        sub_folder_path = f"{self.folder_path}/{n_complete}_{self.cd.num_triples}/"

        while True:
            file_id = get_unprocessed_fileid(sub_folder_path)

            if file_id is not None:
                try:
                    os.rename(sub_folder_path+f"{file_id}.pkl",
                              sub_folder_path+f"{file_id}.processing")

                    trs_score_list = self.load_trs_score_list(f"{n_complete}_{self.cd.num_triples}",
                                                              f"{file_id}.processing")

                    full_trs_score_list = []
                    self.chunk_id = 1
                    for trs, _ in trs_score_list:
                        self.split_id = 1
                        self.start_time = time.time()
                        self.set_triple_dict(trs)
                        from_triple_idx = self.triple_index[tuple(self.cd.next_unassigned_triple(trs))]

                        self.fill_trs(trs,
                                      cutoff,
                                      threshold,
                                      full_trs_score_list,
                                      from_triple_idx,
                                      chunk_size,
                                      file_id)

                    if len(full_trs_score_list) > 0:
                        self.save_trs_score_list(full_trs_score_list,
                                                 f"{self.cd.num_triples}_{self.cd.num_triples}",
                                                 f"{file_id}_{0}.pkl")

                    os.remove(sub_folder_path+f"{file_id}.processing")

                except FileNotFoundError as e:
                    print(e)

            elif len(os.listdir(sub_folder_path)) == 0:
                break


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=11)
parser.add_argument("-rules", nargs="*", type=str, default=["2N1", "2N3"])
parser.add_argument("-cutoff", type=int, default=16)
parser.add_argument("-threshold", type=float, default=0)
parser.add_argument("-n_complete", type=int, default=5)
parser.add_argument("-n_chunks", type=int, default=10)
parser.add_argument("-shuffle", type=bool, default="")
parser.add_argument("-lib_path", type=str, default="/Users/bei/CLionProjects/cdl" if platform.system() == 'Darwin' else "E:/cdl")
parser.add_argument("-result_path", type=str, default="./results")
parser.add_argument("-per_trs_time_limit", type=float, default=0.02)
parser.add_argument("-chunk_size", type=int, default=1000)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd,
                      rules=config['rules'],
                      lib_path=config['lib_path'],
                      result_path=config['result_path'])

# trs = cd.init_trs_random()
# for tr in trs:
#     trs = cd.assign_rule(trs, tr.triple, random.choice(es.rules))
#
# es.set_triple_rule_dict(trs)
# es.triple_rule_dict[(1, 2, 3)] = ["2N3"]
#
# trs = cd.assign_rule(trs, [1, 2, 3], "2N3")
# trs = cd.assign_rule(trs, [1, 2, 4], "2N3")
#
# print_trs(trs)
#
# es.split_trs(trs, 1)
# print(es.triple_rule_dict)

es.static_search(cutoff=config['cutoff'],
                 threshold=config['threshold'],
                 n_complete=config['n_complete'],
                 n_chunks=config['n_chunks'],
                 shuffle=config['shuffle'],
                 per_trs_time_limit=config['per_trs_time_limit'],
                 chunk_size=config["chunk_size"])



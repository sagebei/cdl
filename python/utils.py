import pickle
from collections import Counter
import os
from collections import OrderedDict
import numpy as np


class StaticFeature5:
    def __init__(self, cd, n_rules=4, lib_path="~/cdl"):
        self.cd = cd
        self.cd.init_trs()
        self.cd.init_subset(5)
        if n_rules <= 4:
            with open(f'{lib_path}/databases/database_5.pkl', "rb") as f:
                self.dataset_5 = pickle.load(f)
        else:
            with open(f'{lib_path}/databases/database_5_six_rules.pkl', "rb") as f:
                self.dataset_5 = pickle.load(f)
        self.lib_path = lib_path

    def fetch_feature(self, trs):
        sizes = []
        states = self.cd.subset_states(trs)
        for state in states:
            sizes.append(self.dataset_5[tuple(state)])

        return sizes

    def score_function(self, trs, cutoff=16, threshold=0):
        sizes = self.fetch_feature(trs)

        if min(sizes) < cutoff:
            return -1

        counter = Counter(sizes)
        num_16 = counter.get(16, 0)
        num_17 = counter.get(17, 0)
        num_18 = counter.get(18, 0)
        num_19 = counter.get(19, 0)
        num_20 = counter.get(20, 0)

        score = (0 * num_16) + (1 * num_17) + (2 * num_18) + (3 * num_19) + (4 * num_20)

        if score < threshold:
            return -1

        return score


class Search:
    def __init__(self, cd, rules, lib_path):
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules), lib_path=lib_path)
        self.rules = rules
        self.lib_path = lib_path

    def expand_trs(self,
                   trs,
                   cutoff=16,
                   threshold=0):

        triplet = self.cd.unassigned_triplets(trs)[0]
        trs_value_list = []
        for rule in self.rules:
            trs = self.cd.assign_rule(trs, triplet, rule)
            value = self.sf.score_function(trs, cutoff, threshold)
            trs_value_list.append((trs, value))

        return trs_value_list

    def save_trs_list(self,
                      trs_list,
                      core_id,
                      folder_name,
                      sub_folder_name,
                      filename,
                      remove=True):

        name = sub_folder_name.split("_")
        n_iter, num_unassigned = int(name[0]), int(name[1])

        trs_folder_name = f'{self.lib_path}/python/{self.cd.n}/{folder_name}/{n_iter}_{num_unassigned}/'
        if not os.path.exists(trs_folder_name):
            os.makedirs(trs_folder_name)

        with open(trs_folder_name + filename, "wb") as f:
            pickle.dump(trs_list, f)

        if remove:
            os.remove(f"{self.lib_path}/{self.cd.n}/{folder_name}/{n_iter - 1}_{num_unassigned}/{core_id}.pkl")

    def load_trs_list(self,
                      folder_name,
                      sub_folder_name,
                      filename):

        folder_name = f'{self.lib_path}/{self.cd.n}/{folder_name}/{sub_folder_name}/'
        with open(folder_name + filename, "rb") as f:
            trs_list = pickle.load(f)
        return trs_list

    def get_size_counter(self,
                         folder_name,
                         threshold=0):

        sizes = []
        for filename in os.listdir(f'{self.lib_path}/{self.cd.n}/{folder_name}/{self.cd.num_triplets}_{self.cd.num_triplets}/'):
            trs_score_list = self.load_trs_list(folder_name, f"{self.cd.num_triplets}_{self.cd.num_triplets}", filename)
            for trs, score in trs_score_list:
                if score > threshold:
                    sizes.append(self.cd.size(trs))

        result = Counter(sizes)
        result = OrderedDict(sorted(result.items(), key=lambda t: t[0]))

        return result


def init_rules(cd, trs, low_exceptions, high_exceptions):
    middle = np.array(low_exceptions).max()
    for tr in trs:
        i, j, k = tr.triplet
        if j <= middle < k:
            if (i, j) in low_exceptions:
                trs = cd.assign_rule(trs, tr.triplet, "1N3")
            else:
                trs = cd.assign_rule(trs, tr.triplet, "3N1")
        if i <= middle < j:
            if (j, k) in high_exceptions:
                trs = cd.assign_rule(trs, tr.triplet, "3N1")
            else:
                trs = cd.assign_rule(trs, tr.triplet, "1N3")

    return trs


def flip_exceptions(cd, exceptions):
    flipout = []
    for pair in exceptions:
        flipout.append((cd.n+1-pair[1], cd.n+1-pair[0]))
    return flipout




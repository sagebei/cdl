import pickle
from collections import Counter
import os
from collections import OrderedDict, defaultdict
import shutil


class StaticFeature5:
    def __init__(self, cd, n_rules=4, lib_path="~/cdl"):
        self.cd = cd
        self.cd.init_trs()
        self.cd.init_subset(5)
        if n_rules <= 4:
            with open(f'{lib_path}/python/databases/database_5.pkl', "rb") as f:
                self.dataset_5 = pickle.load(f)
        else:
            with open(f'{lib_path}/python/databases/database_5_six_rules.pkl', "rb") as f:
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
    def __init__(self, cd, rules, lib_path, result_path):
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules), lib_path=lib_path)
        self.rules = rules
        self.lib_path = lib_path
        self.result_path = result_path

    def expand_trs(self,
                   trs,
                   cutoff=16,
                   threshold=0):

        triplet = self.cd.unassigned_triplets(trs)[0]
        trs_value_list = []
        for rule in self.rules:
            trs = self.cd.assign_rule(trs, triplet, rule)
            value = self.sf.score_function(trs, cutoff, threshold)
            if value > -1:
                trs_value_list.append((trs, value))

        return trs_value_list

    def save_trs_list(self,
                      trs_list,
                      folder_name,
                      sub_folder_name,
                      filename,
                      remove=True):

        name = sub_folder_name.split("_")
        n_iter, num_unassigned = int(name[0]), int(name[1])

        trs_folder_name = f'{self.result_path}/{self.cd.n}/{folder_name}/{n_iter}_{num_unassigned}/'
        if not os.path.exists(trs_folder_name):
            os.makedirs(trs_folder_name)

        with open(trs_folder_name + filename, "wb") as f:
            trs_score_list = []
            for trs, score in trs_list:
                state = self.cd.trs_to_state(trs)
                trs_score_list.append((state, score))
            pickle.dump(trs_score_list, f)

        if remove:
            file_id = int(filename.split(".")[0])
            try:
                os.remove(f"{self.result_path}/{self.cd.n}/{folder_name}/{n_iter - 1}_{num_unassigned}/{file_id}.pkl")
            except FileNotFoundError:
                os.remove(f"{self.result_path}/{self.cd.n}/{folder_name}/{n_iter - 1}_{num_unassigned}/{file_id}.processing")

    def load_trs_list(self,
                      folder_name,
                      sub_folder_name,
                      filename):

        folder_name = f'{self.result_path}/{self.cd.n}/{folder_name}/{sub_folder_name}/'
        with open(folder_name + filename, "rb") as f:
            state_score_list = pickle.load(f)

        trs_list = []
        for state, score in state_score_list:
            trs = self.cd.state_to_trs(state)
            trs_list.append((trs, score))
        return trs_list

    def get_size_counter(self,
                         folder_name):

        sizes = []
        trs_score_size_list = []
        for filename in os.listdir(f'{self.result_path}/{self.cd.n}/{folder_name}/{self.cd.num_triplets}_{self.cd.num_triplets}/'):
            trs_score_list = self.load_trs_list(folder_name, f"{self.cd.num_triplets}_{self.cd.num_triplets}", filename)
            for trs, score in trs_score_list:
                size = self.cd.size(trs)
                sizes.append(size)
                trs_score_size_list.append((trs, score, size))

        result = Counter(sizes)
        result = OrderedDict(sorted(result.items(), key=lambda t: t[0]))

        with open(f"{self.result_path}/{self.cd.n}/{folder_name}/trs_score_size.pkl", "wb") as f:
            pickle.dump(trs_score_size_list, f)

        shutil.rmtree(f"{self.result_path}/{self.cd.n}/{folder_name}/{int(folder_name.split('_')[5])}_{self.cd.num_triplets}")

        return result

    def save_result_as_dict(self, folder_name):
        score_states_dict = defaultdict(list)

        for filename in os.listdir(f'{self.result_path}/{self.cd.n}/{folder_name}/{self.cd.num_triplets}_{self.cd.num_triplets}/'):
            trs_score_list = self.load_trs_list(folder_name, f"{self.cd.num_triplets}_{self.cd.num_triplets}", filename)
            for trs, score in trs_score_list:
                score_states_dict[score].append(self.cd.trs_to_state(trs))

        with open(f"{self.result_path}/{self.cd.n}/{folder_name}/result_dict.pkl", "wb") as f:
            pickle.dump(score_states_dict, f)




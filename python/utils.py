import pickle
from collections import Counter
import os
from collections import OrderedDict, defaultdict
from tools import Fishburn_scores


class StaticFeature5:
    def __init__(self, cd, rules, lib_path="~/cdl"):
        self.cd = cd
        self.cd.init_trs()
        self.cd.init_subset(5)
        self.database_folder = f'{lib_path}/python/databases/'

        suited_filename = ""
        num_rules = 6
        for filename in os.listdir(self.database_folder):
            database_rules = filename.split("_")[2:]
            if set(rules).issubset(set(database_rules)):
                if len(database_rules) <= num_rules:
                    num_rules = len(database_rules)
                    suited_filename = filename

        with open(self.database_folder + suited_filename, "rb") as f:
            self.dataset_5 = pickle.load(f)

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

        if score < threshold * Fishburn_scores[self.cd.n]:
            return -1

        return score


class Search:
    def __init__(self, cd, rules, lib_path, result_path):
        self.cd = cd
        self.sf = StaticFeature5(cd, rules=rules, lib_path=lib_path)
        self.rules = rules
        self.lib_path = lib_path
        self.result_path = result_path
        self.folder_path = f"{result_path}/{cd.n}/"

    def expand_trs(self,
                   trs,
                   cutoff=16,
                   threshold=0):

        triple = self.cd.unassigned_triples(trs)[0]
        trs_score_list = []
        for rule in self.rules:
            trs = self.cd.assign_rule(trs, triple, rule)
            score = self.sf.score_function(trs, cutoff, threshold)
            if score > -1:
                trs_score_list.append((trs, score))

        return trs_score_list

    def save_trs_score_list(self,
                            trs_list,
                            sub_folder_name,
                            filename):

        sub_folder_path = f'{self.folder_path}/{sub_folder_name}/'
        if not os.path.exists(sub_folder_path):
            os.makedirs(sub_folder_path)

        with open(sub_folder_path + filename, "wb") as f:
            trs_score_list = []
            for trs, score in trs_list:
                state = self.cd.trs_to_state(trs)
                trs_score_list.append((state, score))
            pickle.dump(trs_score_list, f)

    def load_trs_score_list(self,
                            sub_folder_name,
                            filename):

        sub_folder_path = f'{self.folder_path}/{sub_folder_name}/'
        with open(sub_folder_path + filename, "rb") as f:
            state_score_list = pickle.load(f)

        trs_score_list = []
        for state, score in state_score_list:
            trs = self.cd.state_to_trs(state)
            trs_score_list.append((trs, score))
        return trs_score_list

    def get_size_counter(self):

        sizes = []
        trs_score_size_list = []
        for filename in os.listdir(f'{self.folder_path}/{self.cd.num_triples}_{self.cd.num_triples}/'):
            trs_score_list = self.load_trs_score_list(f"{self.cd.num_triples}_{self.cd.num_triples}", filename)
            for trs, score in trs_score_list:
                size = self.cd.size(trs)
                sizes.append(size)
                trs_score_size_list.append((trs, score, size))

        result = Counter(sizes)
        result = OrderedDict(sorted(result.items(), key=lambda t: t[0]))

        with open(f"{self.folder_path}/trs_score_size.pkl", "wb") as f:
            pickle.dump(trs_score_size_list, f)

        return result

    def save_result_as_dict(self):
        score_states_dict = defaultdict(list)

        for filename in os.listdir(f'{self.folder_path}/{self.cd.num_triples}_{self.cd.num_triples}/'):
            trs_score_list = self.load_trs_score_list(f"{self.cd.num_triples}_{self.cd.num_triples}", filename)
            for trs, score in trs_score_list:
                score_states_dict[score].append(self.cd.trs_to_state(trs))

        with open(f"{self.folder_path}/result_dict.pkl", "wb") as f:
            pickle.dump(score_states_dict, f)




import pickle
from collections import Counter
import os
from collections import OrderedDict, defaultdict
from cdl import Fishburn_scheme


def get_unprocessed_fileid(sub_folder_path, buffer_size=10000):
    filenames = os.listdir(sub_folder_path)

    counter = 0
    unprocessed_ids = []
    for filename in filenames:
        file_id, file_extension = filename.split(".")
        if file_extension == "pkl":
            unprocessed_ids.append(file_id)
            counter += 1

        if counter == buffer_size:
            break

    if len(unprocessed_ids) == 0:
        return None

    return random.choice(unprocessed_ids)


def get_unprocessed_file(sub_folder_path, core_id):
    filenames = os.listdir(sub_folder_path)

    for filename in filenames:
        if filename.split("-")[0] == str(core_id):
            return filename.split(".")[0]
    return None


def divide_labor(sub_folder_path, n_cores):
    filenames = os.listdir(sub_folder_path)
    random.shuffle(filenames)
    core_capacity = math.ceil(len(filenames) / n_cores)
    core_id = 0
    for i, filename in enumerate(filenames):
        if i % core_capacity == 0:
            core_id += 1
        os.rename(sub_folder_path + "/" + filename,
                  sub_folder_path + f"/{core_id}-" + filename)


class StaticFeature5:
    def __init__(self, cd, rules, lib_path="~/cdl"):
        self.cd = cd
        self.cd.init_trs()
        self.cd.init_subset(5)
        self.database_folder = f'{lib_path}/python/databases/'

        suited_filename = ""
        num_rules = 6
        for filename in os.listdir(self.database_folder):
            name, extension = filename.split(".")
            if extension == 'pkl':
                names = name.split("_")
                database_rules = names[2:]
                database_sub_n = int(names[1])

                if database_sub_n == 5:
                    if set(rules).issubset(set(database_rules)):
                        print(filename, set(database_rules))
                        if len(database_rules) <= num_rules:
                            num_rules = len(database_rules)
                            suited_filename = filename
        print("database", self.database_folder + suited_filename)
        with open(self.database_folder + suited_filename, "rb") as f:
            self.dataset_5 = pickle.load(f)

        if "2N1" in rules and "2N3" in rules:
            self.base_score = self._score_function(cd.init_trs_by_scheme(Fishburn_scheme), cutoff=16)
        else:
            self.base_score = 0

    def fetch_feature(self, trs):
        sizes = []
        states = self.cd.subset_states(trs)
        for state in states:
            sizes.append(self.dataset_5[tuple(state)])

        return sizes

    def _score_function(self, trs, cutoff=16):
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

        return score

    def score_function(self, trs, cutoff=16, threshold=0):
        score = self._score_function(trs, cutoff)
        if score != -1:
            if score < self.base_score * threshold:
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

        triple = self.cd.next_unassigned_triple(trs)
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


Fishburn_sizes = {
    4: 9,
    5: 20,
    6: 45,
    7: 100,
    8: 222,
    9: 488,
    10: 1069,
    11: 2324,
    12: 5034,
    13: 10840,
    14: 23266,
    15: 49704,
    16: 105884,
    17: 224720,
    18: 475773,
    19: 1004212,
    20: 2115186,
    21: 4443896,
    22: 9319702,
}


Fishburn_scores = {
    6: 22,
    7: 72,
    8: 184,
    9: 400,
    10: 780,
    11: 1400,
    12: 2360,
    13: 3780,
    14: 5810,
    15: 8624,
    16: 12432,
    17: 17472,
    18: 24024,
    19: 32400,
    20: 42960,
    21: 56100,
    22: 72270,
    23: 91960,
    24: 115720,
    25: 144144,
    26: 177892,
    27: 217672,
    28: 264264,
    29: 318500,
    30: 381290,
}


Highest_Records = {
    4: 9,
    5: 20,
    6: 45,
    7: 100,
    8: 224,
    9: 492,
    10: 1090,
    11: 2452,
    12: 5520,
}



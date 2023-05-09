from cdl import *
import argparse
import pickle
import os
from tools import get_unprocessed_fileid
from collections import Counter


def calculate_size(cd, folder_path, filename):
    trs_score_size_list = []
    data_file = f"{folder_path}/{cd.num_triplets}_{cd.num_triplets}/{filename}"
    with open(data_file, "rb") as f:
        state_score_list = pickle.load(f)
        for state, score in state_score_list:
            trs = cd.state_to_trs(state)
            size = cd.size(trs)
            trs_score_size_list.append((trs, score, size))

    result_folder_path = f"{folder_path}/trs_score_size/"
    if not os.path.exists(result_folder_path):
        os.makedirs(result_folder_path)

    with open(result_folder_path + f"{filename.split('.')}.pkl", "wb") as f:
        pickle.dump(trs_score_size_list, f)

    os.remove(data_file)


parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-folder_path", type=str)
parser.add_argument("-n_cores", type=int)
parser.add_argument("-core_id", type=int)
args = parser.parse_args()
config = vars(args)
print(config)

folder_path = config['folder_path']
core_id = config['core_id']
n_cores = config['n_cores']

folders = folder_path.split("/")
if folders[-1] == "":
    folders = folders[:-1]
folder_path = "/".join(folders)

n = int(folders[-2])
cd = CondorcetDomain(n)
num_triplets = cd.num_triplets

calculate_size(cd, folder_path, f"{core_id}.pkl")

sub_folder_path = f"{folder_path}/{cd.num_triplets}_{cd.num_triplets}/"
file_id = get_unprocessed_fileid(sub_folder_path, n_cores)
while file_id is not None:
    os.rename(sub_folder_path+f"{file_id}.pkl",
              sub_folder_path+f"{file_id}.processing")
    calculate_size(cd, folder_path, f"{file_id}.processing")
    file_id = get_unprocessed_fileid(sub_folder_path, n_cores)


if len(os.listdir(sub_folder_path)) == 0:
    sizes = []
    result_folder_path = f"{folder_path}/trs_score_size/"
    filenames = os.listdir(result_folder_path)
    for filename in filenames:
        with open(result_folder_path + filename, "rb") as f:
            trs_score_size_list = pickle.load(f)
            for (trs, score, size) in trs_score_size_list:
                sizes.append(size)

    with open(f"{folder_path}/counter.txt", "w") as f:
        f.write(str(Counter(sizes)))



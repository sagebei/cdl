from cdl import *
import argparse
import pickle
import os
from tools import get_unprocessed_fileid
from collections import Counter
import shutil
from collections import OrderedDict


def calculate_size(cd, folder_path, filename):
    trs_score_size_list = []
    sizes = []
    data_file = f"{folder_path}/{cd.num_triplets}_{cd.num_triplets}/{filename}"
    with open(data_file, "rb") as f:
        state_score_list = pickle.load(f)
        for state, score in state_score_list:
            trs = cd.state_to_trs(state)
            size = cd.size(trs)
            trs_score_size_list.append((trs, score, size))
            sizes.append(size)

    # save trs_score_size list
    trs_score_size_folder_path = f"{folder_path}/trs_score_size/"
    try:
        if not os.path.exists(trs_score_size_folder_path):
            os.makedirs(trs_score_size_folder_path)
    except FileExistsError as e:
        print(e)

    with open(trs_score_size_folder_path + f"{filename.split('.')[0]}.pkl", "wb") as f:
        pickle.dump(trs_score_size_list, f)

    # save sizes
    sizes_folder_path = f"{folder_path}/sizes/"
    try:
        if not os.path.exists(sizes_folder_path):
            os.makedirs(sizes_folder_path)
    except FileExistsError as e:
        print(e)

    with open(sizes_folder_path + f"{filename.split('.')[0]}.pkl", "wb") as f:
        pickle.dump(sizes, f)

    # remove the file that has been processed.
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

sub_folder_path = f"{folder_path}/{cd.num_triplets}_{cd.num_triplets}/"
file_id = get_unprocessed_fileid(sub_folder_path, n_cores)
while file_id is not None:
    try:
        os.rename(sub_folder_path+f"{file_id}.pkl",
                  sub_folder_path+f"{file_id}.processing")
        calculate_size(cd,
                       folder_path,
                       f"{file_id}.processing")
        os.remove(sub_folder_path+f"{file_id}.processing")

    except Exception as e:
        print(e)

    file_id = get_unprocessed_fileid(sub_folder_path, n_cores)


if len(os.listdir(sub_folder_path)) == 0:
    # build and save the size counter
    sizes = []
    result_folder_path = f"{folder_path}/sizes/"
    filenames = os.listdir(result_folder_path)
    for filename in filenames:
        with open(result_folder_path + filename, "rb") as f:
            sizes.extend(pickle.load(f))

    with open(f"{folder_path}/counter.txt", "w") as f:
        result = Counter(sizes)
        result = OrderedDict(sorted(result.items(), key=lambda t: t[0]))
        f.write(str(result))

    # remove unused folders
    shutil.rmtree(f"{folder_path}/sizes/")
    for i in range(num_triplets + 1):
        try:
            shutil.rmtree(folder_path + f"/{i}_{num_triplets}")
        except FileNotFoundError:
            pass




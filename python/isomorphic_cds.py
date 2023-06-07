from cdl import *
import argparse
from tools import get_unprocessed_fileid
import pickle
import os
from collections import Counter


parser = argparse.ArgumentParser(description="find the isomorphic CDs",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-folder_path", type=str, default="./results/6/14_0_100000_10_10_20_False_1N3_2N3")
args = parser.parse_args()
config = vars(args)
print(config)

folder_path = config['folder_path']
result_path = folder_path + "/isomorphic_cds/"

if not os.path.exists(result_path):
    os.makedirs(result_path)

folders = folder_path.split("/")
if folders[-1] == "":
    folders = folders[:-1]
folder_path = "/".join(folders)

n = int(folders[-2])
cd = CondorcetDomain(n)

sub_folder_path = f"{folder_path}/{cd.num_triplets}_{cd.num_triplets}/"
file_id = get_unprocessed_fileid(sub_folder_path)
while file_id is not None:
    # try:
    os.rename(sub_folder_path+f"{file_id}.pkl",
              sub_folder_path+f"{file_id}.processing")

    with open(sub_folder_path+f"{file_id}.processing", "rb") as f:
        state_score_list = pickle.load(f)
        iso_hashes = []
        for (state, score) in state_score_list:
            trs = cd.state_to_trs(state)
            domain = cd.condorcet_domain(trs)
            iso_hash = cd.isomorphic_cd_hash(domain)
            iso_hashes.append(iso_hash)
        with open(result_path+f"{file_id}.pkl", "wb") as fr:
            pickle.dump(iso_hashes, fr)

    os.remove(sub_folder_path+f"{file_id}.processing")

    # except Exception as e:
    #     print(e)

    file_id = get_unprocessed_fileid(sub_folder_path)


if len(os.listdir(sub_folder_path)) == 0:
    # build and save the size counter
    iso_hashes = []
    filenames = os.listdir(result_path)
    for filename in filenames:
        datafile = result_path + filename
        with open(datafile, "rb") as f:
            iso_hashes.extend(pickle.load(f))
        os.remove(datafile)

    with open(f"{result_path}/counter.txt", "w") as f:
        result = Counter(iso_hashes)
        f.write(str(result))

















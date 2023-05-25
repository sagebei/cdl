import random
import numpy as np
import os


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


def get_unprocessed_fileid(sub_folder_path, n_cores):
    filenames = os.listdir(sub_folder_path)
    unprocessed_ids = []
    for filename in filenames:
        file_id, file_extension = filename.split(".")
        file_id = int(file_id)
        if file_id > n_cores and file_extension == "pkl":
            unprocessed_ids.append(file_id)

    if len(unprocessed_ids) == 0:
        return None

    return random.choice(unprocessed_ids)


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



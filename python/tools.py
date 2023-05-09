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


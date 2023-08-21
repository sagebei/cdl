import random
import numpy as np
import os


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



from cdl import *
from pathlib import Path
import argparse
import pickle
from collections import Counter

parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=20)
parser.add_argument("-input_path", type=Path, default="../../")

args = parser.parse_args()
config = vars(args)
print(config)

n = config['n']
input_path = config['input_path'] / "python" / "properties"

result_path = input_path / "results"
result_path.mkdir(exist_ok=True, parents=True)
result_file = result_path / "alternative.txt"



cd = CondorcetDomain(n)
record_trss = input_path / "record_trss.pkl"
with open(record_trss, 'rb') as f:
    trss = pickle.load(f)
    trs_list = trss[cd.n]
    # print(trs_list)
    for trs in trs_list:
        top_1 = []
        top_n = []
        # print(trs)
        domain = cd.domain(trs)
        # print([i for i in range(1, n+1)] in domain)
        for permutation in domain:
            top_1.append(permutation[0])
            top_n.append(permutation[-1])

        with result_file.open('a') as f:
            f.write(f"n={n}, size={len(domain)}, alternative={len(Counter(top_1)), len(Counter(top_n))}\n")





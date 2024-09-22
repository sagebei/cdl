from cdl import *
from pathlib import Path
import argparse
import pickle
from utils import count_rule
import networkx as nx

parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=9)
parser.add_argument("-input_path", type=Path, default="../../")

args = parser.parse_args()
config = vars(args)
print(config)

n = config['n']
input_path = config['input_path'] / "python" / "properties"

result_path = input_path / "results"
result_path.mkdir(exist_ok=True, parents=True)
result_file = result_path / "count.txt"

cd = CondorcetDomain(n)
record_trss = input_path / "record_trss.pkl"


with open(record_trss, 'rb') as f:
    trss = pickle.load(f)
    trs_list = trss[cd.n]
    for trs in trs_list:
        count = 10000000000
        domain = cd.domain(trs)

        for permutation in cd.domain(trs):
            new_trs = cd.inverse_trs(trs, permutation, ["1N3", "3N1", "2N3", "2N1"])
            if len(new_trs) != 0:
                iso_state = cd.trs_to_state(new_trs)
                if count > count_rule(iso_state):
                    count = count_rule(iso_state)


        with result_file.open('a') as f:
            f.write(f"n={n}, size={len(domain)}, count={count}\n")







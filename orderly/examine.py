from cdl import CondorcetDomain
import argparse
from pathlib import Path
from collections import Counter
import argparse
import pickle
from tqdm import tqdm
from latex import dict_to_latex_table


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=9)
args = parser.parse_args()
config = vars(args)

states = []
sizes = []
cd = CondorcetDomain(args.n)
results = Path(f"colex_1n33n1/{cd.n}/{cd.num_triples}_{cd.num_triples}")
size_list = []
iso_state_size_set = set()
for filepath in results.iterdir():  
    with filepath.open("rb") as f:
        data = pickle.load(f)
        for state, size in data:
            iso_state_size_set.add(tuple(state))
            size_list.append(size)


data = dict(sorted(Counter(size_list).items()))

print(dict_to_latex_table(data, block_size=17))


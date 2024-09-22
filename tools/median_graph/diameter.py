from cdl import *
from pathlib import Path
import argparse
import pickle
from utils import build_graph
import networkx as nx
from networkx.algorithms import approximation

parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=17)
parser.add_argument("-input_path", type=Path, default="./")

args = parser.parse_args()
config = vars(args)
print(config)

n = config['n']
input_path = config['input_path'] / "python" / "properties"

result_path = input_path / "results"
result_path.mkdir(exist_ok=True, parents=True)
result_file = result_path / "diameter_approx.txt"

cd = CondorcetDomain(n)
record_trss = input_path / "record_trss.pkl"
with open(record_trss, 'rb') as f:
    trss = pickle.load(f)
    trs_list = trss[cd.n]
    for trs in trs_list:
        domain = cd.domain(trs)
        G = build_graph(cd, domain)

        with result_file.open('a') as f:
            # f.write(f"n={n}, size={len(domain)}, diameter={nx.diameter(G)}\n")
            f.write(f"n={n}, size={len(domain)}, diameter={approximation.diameter(G)}\n")




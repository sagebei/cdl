from cdl import *
from utils import Search
import argparse
import os

parser = argparse.ArgumentParser(description="complete search for the first n triplet",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int)
parser.add_argument("-cutoff", type=int)
parser.add_argument("-threshold", type=int)
parser.add_argument("-rules", nargs="*", type=str)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = Search(cd, rules=config['rules'])
folder_name = f"{config['cutoff']}_" + f"_".join(config['rules'])
results = es.get_size_counter(folder_name, config['threshold'])
print(results)

if not os.path.exists("results"):
    os.makedirs("results")

with open(f"./results/{folder_name}.txt", "w") as f:
    f.write(str(results))

# python get_results.py -n 6 -cutoff 16 -threshold 0 -rules "2N3" "2N1"


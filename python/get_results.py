from cdl import *
from utils import Search
import argparse
import os

parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int)
parser.add_argument("-cutoff", type=int)
parser.add_argument("-threshold", type=int)
parser.add_argument("-rules", nargs="*", type=str)
parser.add_argument("-lib_path", type=str)
parser.add_argument("-result_path", type=str)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = Search(cd, rules=config['rules'], lib_path=config['lib_path'], result_path=config['result_path'])
folder_name = f"{config['cutoff']}_" + f"_".join(config['rules'])
results = es.get_size_counter(folder_name, config['threshold'])
print(results)

if not os.path.exists(f"{config['result_path']}/counters"):
    os.makedirs(f"{config['result_path']}/counters")

with open(f"{config['result_path']}/counters/{folder_name}.txt", "w") as f:
    f.write(str(results))

# python get_results.py -n 6 -cutoff 16 -threshold 0 -rules "2N3" "2N1"


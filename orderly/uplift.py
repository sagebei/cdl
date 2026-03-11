import random
from cdl import *
from utils import Search
import argparse
import sys
import pickle
from pathlib import Path
sys.setrecursionlimit(5000)
from utils import split_into_chunks, TDS
from tqdm import tqdm


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules, result_path):
        super().__init__(cd, rules, result_path)
        self.filepath = Path("")
        self.n_complete = 0
        self.largest_size_found = 0
        self.count = 1


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=5)
parser.add_argument("-rules", nargs="*", type=str, default=["1N3", "3N1"])
parser.add_argument("-cutoff_abundance", type=int, default=0)
parser.add_argument("-cutoff_size", type=int, default=0)
parser.add_argument("-result_path", type=Path, default="./colex_1n33n1")
parser.add_argument("-i", type=int, default=0)
parser.add_argument("--verbose", action="store_true")
args = parser.parse_args()
config = vars(args)

cd7 = CondorcetDomain(n=config['n'])
cd8 = CondorcetDomain(n=config['n']+1)
es = ExhaustiveSearch(cd7,
                      rules=config['rules'],
                      result_path=config['result_path'])


result_path = Path(f"colex_1n33n1/{cd7.n}/{cd7.num_triples}_{cd7.num_triples}")

results = []
for filepath in tqdm(result_path.iterdir()):  
    for tds in es.load(filepath):
        # trs7 = cd7.state_to_trs(cd7.init_trs_colex(), cd7.trs_to_state(tds.trs))

        for trs7 in cd7.isomorphic_trs_list(tds.trs, ["1N3", "3N1"]):
            trs8 = cd8.init_trs_colex()
            tds8 = TDS(trs=[], domain=[], size=-1)
            for tr in trs7:
                trs8 = cd8.assign_id(trs8, tr.triple, tr.rule_id)
            
            tds8.trs = trs8
            results.append(tds8)

sub_folder_path = Path(f"./colex_1n33n1/{cd8.n}/{cd7.num_triples}_{cd8.num_triples}")
sub_folder_path.mkdir(parents=True, exist_ok=True)

random.shuffle(results)
print(len(results))
tds_list_chunks = split_into_chunks(results, 10000)
for i, chunk in enumerate(tds_list_chunks):
    filepath = sub_folder_path / f"{i+1}.pkl"

    trs_size_list = []
    with filepath.open("wb") as f:
        for tds in chunk:
            trs_size_list.append((cd8.trs_to_state(tds.trs), tds.size))
        pickle.dump(trs_size_list, f)
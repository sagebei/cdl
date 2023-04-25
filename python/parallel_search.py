from cdl import *
from utils import StaticFeature5, Search
import argparse
from tqdm import tqdm


class ExhaustiveSearch(Search):
    def __init__(self, cd, rules):
        super().__init__(cd, rules)
        self.cd = cd
        self.sf = StaticFeature5(cd, n_rules=len(rules))
        self.rules = rules

    def static_search(self,
                      cutoff=16,
                      triplet_id=5,
                      core_id=1):

        folder_name = f"{cutoff}_" + f"_".join(self.rules)
        trs_list = self.load_trs_list(folder_name,
                                      f"{triplet_id-1}_{self.cd.num_triplets}",
                                      f"{core_id}.pkl")

        for n_iter in range(triplet_id, cd.num_triplets+1):
            print(f"{core_id}_{n_iter}")
            next_trs_value_list = []

            for trs in trs_list:
                trs_value_list = self.expand_trs(trs, cutoff)
                next_trs_value_list.extend(trs_value_list)

            trs_list.clear()
            trs_list = [trs for (trs, score) in next_trs_value_list]

            self.save_trs_list(trs_list,
                               core_id,
                               folder_name,
                               f"{n_iter}_{cd.num_triplets}",
                               f"{core_id}.pkl",
                               remove=True)


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int)
parser.add_argument("-rules", nargs="*", type=str)
parser.add_argument("-cutoff", type=int)
parser.add_argument("-triplet_id", type=int)
parser.add_argument("-core_id", type=int)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'])
es.static_search(cutoff=config['cutoff'],
                 triplet_id=config['triplet_id'],
                 core_id=config['core_id'])


# python parallel_search.py -n 6 -cutoff 16 -rules "2N3" "2N1"  -triplet_id 6 -core_id 2



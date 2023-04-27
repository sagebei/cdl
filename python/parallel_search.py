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
                      top_n=1000,
                      triplet_id=5,
                      core_id=1):

        folder_name = f"{cutoff}_" + f"_".join(self.rules)
        trs_score_list = self.load_trs_list(folder_name,
                                            f"{triplet_id-1}_{self.cd.num_triplets}",
                                            f"{core_id}.pkl")

        for n_iter in range(triplet_id, cd.num_triplets+1):
            print(f"{core_id}_{n_iter}")
            next_trs_score_list = []

            for trs, _ in trs_score_list:
                trs_value_list = self.expand_trs(trs, cutoff)
                next_trs_score_list.extend(trs_value_list)

            trs_score_list.clear()
            next_trs_score_list.sort(key=lambda trs_score: trs_score[1])
            trs_score_list = next_trs_score_list[-top_n:]

            self.save_trs_list(trs_score_list,
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
parser.add_argument("-top_n", type=int)
parser.add_argument("-triplet_id", type=int)
parser.add_argument("-core_id", type=int)
args = parser.parse_args()
config = vars(args)
print(config)

cd = CondorcetDomain(n=config['n'])
es = ExhaustiveSearch(cd, rules=config['rules'])
es.static_search(cutoff=config['cutoff'],
                 top_n=config['top_n'],
                 triplet_id=config['triplet_id'],
                 core_id=config['core_id'])


# python parallel_search.py -n 6 -cutoff 16 -top_n 1000 -rules "2N3" "2N1"  -triplet_id 6 -core_id 2



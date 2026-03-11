from cdl import *
from utils import Search
import argparse
import sys
import time
import re
from pathlib import Path
from complete_search import Splitter
sys.setrecursionlimit(5000)

day = 60 * 60 * 24

class ParallelSearch(Search):
    def __init__(self, cd, splitter, rules, result_path):
        super().__init__(cd, rules, result_path)
        self.filepath = Path("")
        self.splitter = splitter
        self.start_time = time.time()
        self.count = 0

    def fill_tds(self,
                 tds,
                 i,
                 full_tds_list,
                 cutoff_abundance=16,
                 cutoff_size=488,
                 verbose=False):
        
        if time.time() - self.start_time > day:
            folder = i.parent.parent / f"{self.cd.num_triples}_{self.cd.num_triples}"
            for f in folder.glob(f"{i.stem}*"):
                f.unlink()

            self.splitter.split(self.tds, n_complete=20, filename=i.stem)
            raise Exception()

        triple = self.next_triple(tds.trs)
        if triple == [0, 0, 0]:
            full_tds_list.append(tds)

            if len(full_tds_list) >= 100:
                folder = f"{self.cd.num_triples}_{self.cd.num_triples}"
                self.save(full_tds_list, folder, f"{i.stem}_{self.count}.pkl")
                full_tds_list.clear()
                self.count += 1
        else:
            for rule in self.rules:
                new_tds = self.build_new_tds(tds, triple, rule, cutoff_size)
                if self.check_tds(new_tds, cutoff_abundance, cutoff_size):
                    self.fill_tds(new_tds,
                                  i,
                                  full_tds_list,
                                  cutoff_abundance,
                                  cutoff_size,
                                  verbose)
                else:
                    continue

    def static_search(self,
                      cutoff_abundance=16,
                      cutoff_size=100,
                      verbose=False,
                      i=""):
        
        filepath = i
        ################################################################################
        full_tds_list = []
        tds_list = self.load(filepath)
        for tds in tds_list:
            self.tds = tds
            try:
                self.fill_tds(tds, i, full_tds_list, cutoff_abundance, cutoff_size, verbose)
            except Exception:
                i.unlink()
                full_tds_list.clear()
                continue
        folder = f"{self.cd.num_triples}_{self.cd.num_triples}"
        if len(full_tds_list) > 0:
            self.save(full_tds_list, folder, f"{i.stem}_{self.count}.pkl")
        i.unlink()
        ###############################################################################


parser = argparse.ArgumentParser(description="Run search on a single CPU core",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=9)
parser.add_argument("-rules", nargs="*", type=str, default=["1N3", "3N1"])
parser.add_argument("-cutoff_abundance", type=int, default=0)
parser.add_argument("-cutoff_size", type=int, default=0)
parser.add_argument("-result_path", type=Path, default="./colex_1n33n1")
parser.add_argument("-i", type=Path)
parser.add_argument("--verbose", action="store_true")
args = parser.parse_args()
config = vars(args)


cd = CondorcetDomain(n=config['n'])
splitter = Splitter(cd, rules=config['rules'], result_path=config['result_path'])

es = ParallelSearch(cd,
                    splitter = splitter,
                    rules=config['rules'],
                    result_path=config['result_path'])

es.static_search(cutoff_abundance=config['cutoff_abundance'],
                 cutoff_size=config['cutoff_size'],
                 verbose=config['verbose'],
                 i=config['i'])

# parallel -j 128 python parallel_search.py -n 8 -i {} ::: /home/bzhou6/1n33n1/colex_1n33n1/8/5_56/*


# find /home/bzhou6/1n33n1/colex_1n33n1/9/40_84 -mindepth 1 -maxdepth 1 -type f | parallel --bar -j 500 python parallel_search.py -n 9 -i {}
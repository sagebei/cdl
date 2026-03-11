from cdl import *
import argparse
import numpy as np
import random
from pathlib import Path
from utils import TDS, Search, split_into_chunks


class Splitter(Search):
    def __init__(self, cd, rules, result_path):
        super().__init__(cd, rules, result_path)
        self.prioritised_triples = []

    def split(self,
              tds,
              cutoff_abundance=16,
              cutoff_size=0,
              n_complete=20,
              n_chunks=1000,
              filename=""):

        if n_complete == -1:
            n_complete = cd.num_triples

        num_assigned = len(self.cd.assigned_triples(tds.trs))
        num_unassigned = len(self.cd.unassigned_triples(tds.trs))

        if n_complete > num_unassigned:
            n_complete = num_unassigned

        tds_list = self.expand_tds(tds, cutoff_abundance, cutoff_size)

        for n_iter in range(num_assigned+2, num_assigned+n_complete+1):
            next_tds_list = []

            for tds in tds_list:
                next_tds_list.extend(self.expand_tds(tds, cutoff_abundance, cutoff_size))

            tds_list.clear()
            tds_list = next_tds_list

        random.shuffle(tds_list)

        print(len(tds_list))
        # tds_list_chunks = split_into_chunks(tds_list, n_chunks)

        for i, tds in enumerate(tds_list):
            if filename == "":
                self.save(tds_list=[tds],
                        sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triples}",
                        filename=f"{i+1}.pkl")
            else:
                self.save(tds_list=[tds],
                        sub_folder_name=f"{num_assigned+n_complete}_{self.cd.num_triples}",
                        filename=f"{filename}_{i+1}.pkl")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="complete search for the first n triple",
                                    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-n", type=int, default=9)
    parser.add_argument("-rules", nargs="*", type=str, default=["1N3", "3N1"])
    parser.add_argument("-cutoff_abundance", type=int, default=0)
    parser.add_argument("-cutoff_size", type=int, default=0)
    parser.add_argument("-n_complete", type=int, default=20)
    parser.add_argument("-n_chunks", type=int, default=100000)
    parser.add_argument("-result_path", type=Path, default="./colex_1n33n1")
    args = parser.parse_args()
    config = vars(args)
    print(config)


    cd = CondorcetDomain(n=config['n'])
    es = Splitter(cd, rules=config['rules'], result_path=config['result_path'])

    trs = cd.init_trs_colex()
    tds = TDS(trs, cd.domain(trs), cd.size(trs))
    es.split(tds,
            cutoff_abundance=config['cutoff_abundance'],
            cutoff_size=config['cutoff_size'],
            n_complete=config['n_complete'],
            n_chunks=config['n_chunks'])





#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request 1 CPU core
#$ -l h_vmem=8G         # 8G RAM per core
#$ -l h_rt=1:0:0        # Max 1hr runtime
#$ -N complete          # name of the job


# load the modules
module load GCC/11.3.0
# activate the environment
source ~/store/KLAS/CONDORCET-NEW/venv/bin/activate

# lib_path: where the library is downloaded
# result_path: where the results of the search will be saved
python ../python/complete_search.py -n 7  \
                                    -cutoff 16  \
                                    -threshold 0  \
                                    -rules "2N3" "2N1" "1N3" "3N1" \
                                    -n_complete 5 \
                                    -n_cores 16  \
                                    -lib_path "/home/k/klasm/store/KLAS/CONDORCET-NEW/cdl" \
                                    -result_path "/home/k/klasm/store/KLAS/CONDORCET-NEW/cdl/results"

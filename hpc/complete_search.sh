#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=8G         # 11G RAM per core
#$ -l h_rt=1:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N complete

module load gcc/12.1.0
source ~/venv/bin/activate

python ../python/complete_search.py -n 8  \
                                    -cutoff 16  \
                                    -threshold 0  \
                                    -rules "2N3" "2N1" "1N3" "3N1" \
                                    -n_complete 7 \
                                    -n_cores 100  \
                                    -lib_path "/data/home/acw554/cdl" \
                                    -result_path "/data/scratch/acw554/results"

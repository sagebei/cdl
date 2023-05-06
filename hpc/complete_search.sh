#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=8G         # 11G RAM per core
#$ -l h_rt=1:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N complete

module load gcc/12.1.0
source ~/venv/bin/activate

python ~/cdl/python/complete_search.py -n 7  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n 1000  \
                                       -n_complete 9 \
                                       -n_cores 2000  \
                                       -n_chunks 100000 \
                                       -shuffle 0 \
                                       -rules "2N3" "2N1" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/results"

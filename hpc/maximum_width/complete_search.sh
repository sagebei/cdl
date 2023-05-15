#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=8G         # 11G RAM per core
#$ -l h_rt=24:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N complete

module load gcc/12.1.0
source ~/venv/bin/activate

python ~/cdl/python/complete_search.py -n 20  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n 1000000000  \
                                       -n_complete 56 \
                                       -n_cores 1000  \
                                       -n_chunks 1000000 \
                                       -shuffle 1 \
                                       -rules "2N3" "2N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/maximum_width"

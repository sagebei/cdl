#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=32G         # 11G RAM per core
#$ -l h_rt=24:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N complete

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/complete_search.py -n  $1 \
                                       -cutoff 14  \
                                       -threshold 0  \
                                       -top_n -1  \
                                       -n_complete 30 \
                                       -n_chunks 10000 \
                                       -shuffle 1 \
                                       -rules "1N3" "2N3" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/1N3_2N3"

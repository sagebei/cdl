#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=24:0:0
#$ -N result

module load gcc/12.1.0
source ~/venv/bin/activate

python ~/cdl/python/get_results.py -n 9  \
                                   -cutoff 16  \
                                   -threshold 0  \
                                   -top_n 100000  \
                                   -n_complete 7 \
                                   -n_cores 2000  \
                                   -n_chunks 10000 \
                                   -shuffle 0 \
                                   -rules "2N3" "2N1" "1N3" "3N1" \
                                   -lib_path "/data/home/acw554/cdl" \
                                   -result_path "/data/scratch/acw554/results"
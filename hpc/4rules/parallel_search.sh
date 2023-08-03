#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-100
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n 8  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n 10000  \
                                       -n_complete 20 \
                                       -n_chunks 1000 \
                                       -shuffle "." \
                                       -rules "2N1" "2N3" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/4rules"



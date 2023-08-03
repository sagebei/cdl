#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=32G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-100
#$ -N xparallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n $1  \
                                       -cutoff $2  \
                                       -threshold 0  \
                                       -top_n -1  \
                                       -n_complete -1 \
                                       -n_chunks 100 \
                                       -shuffle "." \
                                       -rules "2N1" "2N3" "1N2" "3N2" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/5x_abundance"



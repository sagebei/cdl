#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=16G
#$ -j y
#$ -l h_rt=240:0:0
#$ -t 1-1000
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n $1  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n -1  \
                                       -n_complete 12 \
                                       -n_chunks 10000 \
                                       -shuffle "." \
                                       -rules "2N1" "2N3" "1N2" "3N2" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/maximum_width"



#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=64G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-100
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n $1  \
                                       -cutoff 14  \
                                       -threshold 0  \
                                       -top_n 10000000000  \
                                       -n_complete 30 \
                                       -n_chunks 10000 \
                                       -shuffle 1 \
                                       -rules "1N3" "2N3" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/1N3_2N3" \



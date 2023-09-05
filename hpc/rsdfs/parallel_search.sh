#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=240:0:0
#$ -t 1-1000
#$ -N rfp

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/RSDFS/parallel_search.py -n $1  \
                                             -cutoff 16  \
                                             -threshold 0  \
                                             -n_complete 60 \
                                             -n_chunks 10000 \
                                             -per_trs_time_limit 180 \
                                             -chunk_size=1000 \
                                             -shuffle "." \
                                             -rules "2N1" "2N3" \
                                             -lib_path "/data/home/acw554/cdl" \
                                             -result_path "/data/scratch/acw554/rsdfs" \




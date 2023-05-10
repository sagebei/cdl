#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=100:0:0
#$ -t 1-1000
#$ -N parallel

module load gcc/12.1.0
source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n 9  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n 100000  \
                                       -n_complete 13 \
                                       -n_cores 1000  \
                                       -n_chunks 10000 \
                                       -shuffle "." \
                                       -rules "2N3" "2N1" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/results" \
                                       -core_id ${SGE_TASK_ID}

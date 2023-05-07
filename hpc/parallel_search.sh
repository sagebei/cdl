#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=10:0:0
#$ -t 1-2000
#$ -N parallel

module load gcc/12.1.0
source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n 7  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n 1000  \
                                       -n_complete 9 \
                                       -n_cores 2000  \
                                       -n_chunks 2000 \
                                       -shuffle 0 \
                                       -rules "2N3" "2N1" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/results" \
                                       -core_id ${SGE_TASK_ID}
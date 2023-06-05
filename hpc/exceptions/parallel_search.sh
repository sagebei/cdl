#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-1000
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n 12  \
                                       -cutoff 16  \
                                       -threshold 0.9  \
                                       -top_n 1000000  \
                                       -n_complete 15 \
                                       -n_cores 1000  \
                                       -n_chunks 10000 \
                                       -shuffle 1 \
                                       -rules "2N3" "2N1" "1N3" "3N1" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/exceptions" \
                                       -core_id ${SGE_TASK_ID}




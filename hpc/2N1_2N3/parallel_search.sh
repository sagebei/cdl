#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=32G
#$ -j y
#$ -l h_rt=150:0:0
#$ -t 1-1000
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/parallel_search.py -n 21  \
                                       -cutoff 16  \
                                       -threshold 0.9  \
                                       -top_n 1000000000000  \
                                       -n_complete 90 \
                                       -n_cores 1000  \
                                       -n_chunks 1000000 \
                                       -shuffle 1 \
                                       -rules "2N1" "2N3" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/maximum_width" \
                                       -core_id ${SGE_TASK_ID}


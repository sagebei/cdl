#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=10:0:0
#$ -t 1-1000
#$ -N parallel

module load gcc/12.1.0
source ~/venv/bin/activate

python ../python/parallel_search.py -n 8  \
                                    -cutoff 16  \
                                    -threshold 100 \
                                    -top_n 10000  \
                                    -n_complete 7 \
                                    -n_cores 1000 \
                                    -rules "2N3" "2N1" "1N3" "3N1"  \
                                    -lib_path "/data/home/acw554/cdl" \
                                    -result_path "/data/scratch/acw554/results" \
                                    -core_id ${SGE_TASK_ID}
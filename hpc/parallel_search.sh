#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=2:0:0
#$ -t 1-1000
#$ -N parallel

module load gcc/12.1.0
source ~/venv/bin/activate

python ../python/parallel_search.py -n 9  \
                                    -cutoff 16  \
                                    -threshold 0 \
                                    -top_n 10000  \
                                    -rules "2N3" "2N1" "1N3" "3N1"  \
                                    -triplet_id 8 \
                                    -core_id ${SGE_TASK_ID} \
                                    -lib_path "/data/home/acw554/cdl" \
                                    -result_path "/data/scratch/acw554/results"

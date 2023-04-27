#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=2G
#$ -j y
#$ -l h_rt=10:0:0
#$ -t 1-16

module load gcc/12.1.0
source ~/venv/bin/activate

python ../parallel_search.py -n 7 -cutoff 16 -rules "2N3" "2N1" "1N3" "3N1"  -triplet_id 6 -core_id ${SGE_TASK_ID}
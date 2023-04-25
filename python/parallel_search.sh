#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=2G
#$ -j y
#$ -l h_rt=10:0:0
#$ -t 1-8

source ~/venv/bin/activate

python parallel_search.py -n 6 -cutoff 16 -rules "2N3" "2N1"  -triplet_id 6 -core_id ${SGE_TASK_ID} &
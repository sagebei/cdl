#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=1G
#$ -j y
#$ -l h_rt=1:0:0

module load gcc/12.1.0 cmake
source ~/venv/bin/activate

python complete_search.py -n 6 -cutoff 16 -rules "2N3" "2N1" -n_complete 5 -n_cores 8 ;
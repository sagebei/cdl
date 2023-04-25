#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=10:0:0

source ~/venv/bin/activate

python complete_search.py -n 6 -cutoff 16 -rules "2N3" "2N1" -n_complete 5 -n_cores 8 ;
#!/bin/bash

#$ -cwd
#$ -pe smp 8
#$ -l h_vmem=1G
#$ -j y
#$ -l h_rt=1:0:0
#$ -N result

module load gcc/12.1.0
source ~/venv/bin/activate

python ../get_results.py -n 7 -cutoff 16 -rules "2N3" "2N1" "1N3" "3N1"
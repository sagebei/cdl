#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=1:0:0
#$ -N result

module load gcc/12.1.0
source ~/venv/bin/activate

python ../python/get_results.py -n 7  \
                                -cutoff 16  \
                                -threshold 0  \
                                -rules "2N3" "2N1" "1N3" "3N1" \
                                -lib_path "~/store/KLAS/CONDORCET-NEW/cdl" \
                                -result_path "~/store/KLAS/CONDORCET-NEW/cdl/results"
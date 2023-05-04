#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=1:0:0
#$ -N result

module load GCC/11.3.0
source ~/store/KLAS/CONDORCET-NEW/venv/bin/activate


python ../python/get_results.py -n 7  \
                                -cutoff 16  \
                                -threshold 0  \
                                -rules "2N3" "2N1" "1N3" "3N1" \
                                -lib_path "/home/k/klasm/store/KLAS/CONDORCET-NEW/cdl" \
				-result_path "/home/k/klasm/store/KLAS/CONDORCET-NEW/cdl/results"

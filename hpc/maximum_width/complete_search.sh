#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=64G         # 11G RAM per core
#$ -l h_rt=24:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N complete

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/PRS/complete_search.py -n $1  \
                                           -cutoff 16  \
                                           -threshold 0  \
                                           -top_n -1  \
                                           -n_complete $2 \
                                           -n_chunks 300000 \
                                           -shuffle "." \
                                           -rules "2N1" "2N3" "1N2" "3N2" \
                                           -lib_path "/data/home/acw554/cdl" \
                                           -result_path "/data/scratch/acw554/maximum_width"

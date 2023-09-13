#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 1            # Request cores (8 per GPU)
#$ -l h_vmem=8G         # 11G RAM per core
#$ -l h_rt=1:0:0      # Max 1hr runtime (can request up to 240hr)
#$ -N rfc

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/RSDFS/complete_search.py -n $1  \
                                             -cutoff 16  \
                                             -threshold 0  \
                                             -n_complete $2 \
                                             -n_chunks 100 \
                                             -shuffle "." \
                                             -rules "1N3" "2N3" \
                                             -lib_path "/data/home/acw554/cdl" \
                                             -result_path "/data/scratch/acw554/1n3_2n3"

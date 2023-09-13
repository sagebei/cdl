#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=8G
#$ -j y
#$ -l h_rt=1:0:0
#$ -t 1-10
#$ -N rfp

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/RSDFS/parallel_search.py -n $1  \
                                             -cutoff 16  \
                                             -threshold 0  \
                                             -n_complete $2 \
                                             -n_chunks 100 \
                                             -per_trs_time_limit 0.02 \
                                             -chunk_size=1000 \
                                             -shuffle "." \
                                             -rules "2N1" "2N3" \
                                             -n_cores=10 \
                                             -split_depth 5 \
                                             -core_id ${SGE_TASK_ID} \
                                             -lib_path "/data/home/acw554/cdl" \
                                             -result_path "/data/scratch/acw554/rsdfs" \





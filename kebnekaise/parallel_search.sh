#!/bin/bash

#$ -cwd
#$ -pe smp 1         # Request 1 CPU core per job
#$ -l h_vmem=8G      # 8G RAM per core
#$ -j y
#$ -l h_rt=1:0:0     # Max 1hr runtime
#$ -t 1-16           # start a array of 16 jobs whose ID will be 1, 2, ..., 16 respectively.
#$ -N parallel

module load gcc/12.1.0
source ~/venv/bin/activate

python ../python/parallel_search.py -n 7  \
                                    -cutoff 16  \
                                    -threshold 0 \
                                    -top_n 1000  \
                                    -rules "2N3" "2N1" "1N3" "3N1"  \
                                    -triplet_id 6 \
                                    -core_id ${SGE_TASK_ID} \
                                    -lib_path "~/store/KLAS/CONDORCET-NEW/cdl/" \
                                    -result_path "~/store/KLAS/CONDORCET-NEW/cdl/results"

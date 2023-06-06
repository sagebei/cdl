#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=16G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-1000
#$ -N sizes

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/calculate_sizes.py -folder_path $1 \
                                       -n_cores 1000 \
                                       -core_id ${SGE_TASK_ID}


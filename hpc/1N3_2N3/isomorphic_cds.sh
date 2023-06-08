#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=32G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-100
#$ -N iso

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ~/cdl/python/isomorphic_cds.py -folder_path $1


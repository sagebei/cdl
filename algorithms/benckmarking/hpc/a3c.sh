#!/bin/bash

#$ -cwd
#$ -j y
#$ -pe smp 8            # Request cores (8 per GPU)
#$ -l h_vmem=8G         # 11G RAM per core
#$ -l h_rt=12:0:0       # Max 1hr runtime (can request up to 240hr)
#$ -l gpu=1             # request 1 GPU
#$ -N a3c

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python ../a3c.py
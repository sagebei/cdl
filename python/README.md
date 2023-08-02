# Prioritised Restricted Search Algorithms Implementation

## Introduction
Prioritised Research Search is a fast algorithm that has discovered new record-breaking 
Condorcet domains, built on top of the CDL library. 

We recommend using the `Prioritised_Restriction_Search.ipynb` under `notebooks` folder to 
conduce experiment on small number of alternatives. Please refer to the section below on 
how to build a 5 alternative database. For large research projects, it is better to use the commands below to run the search algorithm
in parallel on many CPU cores via array jobs. 


## Running PRS on array jobs
We provide an example to apply the PRS algorthm to do a complete search for the maximum 
width Condorcet domains for 8 alternatives that are build from 4 rules (`2N1`, `2N3`, `1N2`, `3N2`) by 
running it on multiple CPU cores. `TRS` denotes a list of triples with rule assigned. 

1. Generate the database: The first step is to build the n=5 database needed for the value 
function of the search. The database is saved in the `cdl/python/databases/` folder
in which the program will look for it automatically. 
```console
python cdl/python/databases/database_5.py 2N1 2N3 1N2 3N2
```

2. Start the search: the following configuration starts the search for n=8 with the cutoff value
being 16. The threshold is the cutoff on the value from the value function which is not needed here.
The search stops at the 20th triple, specified by `n_complete` for every `TRS` and splits them into 100 buckets. The list of `TRS` 
is not shuffled before being split. 
```console
python cdl/python/complete_search.py -n 8 \
                                     -cutoff 16  \
                                     -threshold 0  \
                                     -top_n -1  \
                                     -n_complete 20 \
                                     -n_chunks 100 \
                                     -shuffle "" \
                                     -rules 2N1 2N3 1N2 3N2 \
                                     -lib_path "/data/home/acw554/cdl" \
                                     -result_path "/data/scratch/acw554/maximum_width"
```

3. Parallelize the search: The following command resumes the search on a single process in a CPU core, processing one buckets
of `TRS` at a time. The more CPU cores engage in, the fast we get the results. As there are 100 buckets of `TRS`, 
it is the most efficient to run it on a array of 100 jobs. 
```console
python cdl/python/parallel_search.py -n 8  \
                                     -cutoff 16  \
                                     -threshold 0  \
                                     -top_n -1  \
                                     -n_complete 20 \
                                     -n_chunks 100 \
                                     -shuffle "" \
                                     -rules 2N1 2N3 1N2 3N2 \
                                     -lib_path "/data/home/acw554/cdl" \
                                     -result_path "/data/scratch/acw554/maximum_width" 
```

4. Calculate the resulting Condorcet domain sizes: the following command starts a process that
calculate the sizes for the TRS in each bucket. Again, it benefits from array jobs as well. 
```console
python cdl/python/calculate_sizes.py -folder_path /data/scratch/acw554/maximum_width/6/16_0_-1_100_56_False_1N3_3N1_2N3_2N1
```

## Example bash script
This bash script submit an array of 100 jobs, each of which runs 
the parallel_search in a CPU core. 
```console

#!/bin/bash

#$ -cwd
#$ -pe smp 1
#$ -l h_vmem=64G
#$ -j y
#$ -l h_rt=24:0:0
#$ -t 1-100
#$ -N parallel

module load python/3.8.5
module load gcc/12.1.0

source ~/venv/bin/activate

python cdl/python/parallel_search.py -n 8  \
                                     -cutoff 16  \
                                     -threshold 0  \
                                     -top_n -1  \
                                     -n_complete 20 \
                                     -n_chunks 100 \
                                     -shuffle "" \
                                     -rules 2N1 2N3 1N2 3N2 \
                                     -lib_path "/data/home/acw554/cdl" \
                                     -result_path "/data/scratch/acw554/maximum_width" 

```

If you have issue running these commands, please feel free to raise an issue. 
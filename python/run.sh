#!/bin/bash

source ~/venv

N_CORES=8

python complete_search.py -n 6 -cutoff 16 -rules "2N3" "2N1" -n_complete 5 -n_cores $N_CORES ;

for (( i = 1; i <= N_CORES; i++ ))
do
python parallel_search.py -n 6 -cutoff 16 -rules "2N3" "2N1"  -triplet_id 6 -core_id $i &
done

wait

python get_results.py -n 6 -cutoff 16 -rules "2N3" "2N1"





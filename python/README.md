# Prioritised Restricted Search Implementation

## Introduction
Prioritised Research Search is a fast algorithm that has discovered new record-breaking 
Condorcet domains, built on top of the CDL library. 

We recommend using the `Prioritised_Restriction_Search.ipynb` under `notebooks` folder to 
 conduce experiment on small number of alternatives. For large research projects, it is better to use the commands below to run the search algorithm
in parallel on many CPU cores via array jobs. 


## Running PRS using array jobs
1. 

```console
python complete_search.py -n 8 \
                          -cutoff 16  \
                          -threshold 0  \
                          -top_n -1  \
                          -n_complete 20 \
                          -n_chunks 100 \
                          -shuffle 1 \
                          -rules "1N3" "3N1" "2N3" "2N1" \
                          -lib_path "/data/home/acw554/cdl" \
                          -result_path "/data/scratch/acw554/search_4_rules"
```

2. 
```console
python parallel_search.py -n 8  \
                          -cutoff 16  \
                          -threshold 0  \
                          -top_n -1  \
                          -n_complete 20 \
                          -n_chunks 100 \
                          -shuffle 1 \
                          -rules "1N3" "3N1" "2N3" "2N1" \
                          -lib_path "/data/home/acw554/cdl" \
                          -result_path "/data/scratch/acw554/search_4_rules" \
```

3. 
```console
python calculate_sizes.py -folder_path /data/scratch/acw554/search_4_rules/
```

If you have issue running these commands, please feel free to raise an issue. 
# Prioritised Restricted Search Implementation

Prioritised Research Search is a fast algorithm that has discovered new record-breaking 
Condorcet domains, built on top of the CDL library. 

1. We recommend  

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


```console
python calculate_sizes.py -folder_path /data/scratch/acw554/search_4_rules/
```
# Prioritised Restricted Search Algorithms Implementation

## Introduction
Prioritised Research Search [Zhou and Riis (2023)](https://arxiv.org/pdf/2303.06524.pdf) is a fast algorithm that has discovered new record-breaking 
Condorcet domains. We implemented this algorithm using the CDL library.

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
python ~/cdl/python/databases/database_5.py -rules 2N1 2N3 1N2 3N2
```

2. Start the search: the following configuration starts the search for n=8 with the cutoff value
being 16. The threshold is the cutoff on the value from the value function which is not needed here.
Setting `top_n` to `-1` keeps all the `TRS`s, negating its effect. If it is set, for example, to 10000, then 
the 10000 `TRS`s ranking in top 10000 in terms of their value will be kept. The search stops at the 20th triple, specified by `n_complete` for every `TRS` and splits them into 100 buckets. The list of `TRS` 
is shuffled before being split to ensure that the `TRS`s in a buckets are not correlated (similar). The bash script: [complete_search.sh](https://github.com/sagebei/cdl/blob/main/hpc/maximum_width/complete_search.sh)
```console
python ~/cdl/python/complete_search.py -n 8  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n -1  \
                                       -n_complete 20 \
                                       -n_chunks 100 \
                                       -shuffle "." \
                                       -rules "2N1" "2N3" "1N2" "3N2" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/maximum_width"
```

3. Parallelize the search: The following command resumes the search on a single process in a CPU core, processing one buckets
of `TRS`s at a time. The more CPU cores engage in, the fast we get the results. As there are 100 buckets of `TRS`s, 
it is the most efficient to run it on a array of 100 jobs. The bash script: [parallel_search.sh](https://github.com/sagebei/cdl/blob/main/hpc/maximum_width/parallel_search.sh)
```console
python ~/cdl/python/parallel_search.py -n 8  \
                                       -cutoff 16  \
                                       -threshold 0  \
                                       -top_n -1  \
                                       -n_complete 20 \
                                       -n_chunks 100 \
                                       -shuffle "." \
                                       -rules "2N1" "2N3" "1N2" "3N2" \
                                       -lib_path "/data/home/acw554/cdl" \
                                       -result_path "/data/scratch/acw554/maximum_width"
```

4. Calculate the resulting Condorcet domain sizes: the following command starts a process that
calculate the sizes for the TRS in each bucket. Again, it benefits from array jobs as well.
The bash script:  [calculate_sizes.sh](https://github.com/sagebei/cdl/blob/main/hpc/maximum_width/calculate_sizes.sh)
```console
python ~/cdl/python/calculate_sizes.py -folder_path /data/scratch/acw554/maximum_width/8/16_0.0_-1_100_20_True_2N1_2N3_1N2_3N2
```
It saves a complete list of (trs, its value, its size) in the folder `trs_score_size`, and a summary of the research results
in a text file named `counter.txt` whose content is show below. 

```python
OrderedDict([(128, 276853), (129, 4876), (130, 20780), (131, 7552), (132, 58970), (133, 4120), (134, 34284), 
             (135, 10760), (136, 134888), (137, 6552), (138, 24668), (139, 6984), (140, 91926), (141, 7400), 
             (142, 47112), (143, 13384), (144, 122850), (145, 6580), (146, 30248), (147, 7000), (148, 58442), 
             (149, 7986), (150, 26004), (151, 6508), (152, 81516), (153, 7422), (154, 25606), (155, 6908), 
             (156, 53886), (157, 6014), (158, 25498), (159, 7204), (160, 38600), (161, 4500), (162, 18376), 
             (163, 4288), (164, 26748), (165, 4398), (166, 12890), (167, 3728), (168, 23790), (169, 3476), 
             (170, 11288), (171, 3414), (172, 13894), (173, 2446), (174, 6972), (175, 2000), (176, 8974), 
             (177, 2002), (178, 6384), (179, 1702), (180, 5852), (181, 1444), (182, 2902), (183, 1272), 
             (184, 4044), (185, 906), (186, 2150), (187, 538), (188, 1750), (189, 618), (190, 1116), (191, 360), 
             (192, 1164), (193, 318), (194, 874), (195, 198), (196, 600), (197, 218), (198, 268), (199, 68), 
             (200, 232), (201, 86), (202, 130), (203, 62), (204, 66), (205, 24), (206, 74), (208, 24), (209, 28), 
             (210, 16), (211, 6), (212, 22), (213, 8), (216, 4), (217, 2), (218, 4), (222, 2)])
```


If you have issue running these commands, please feel free to raise an issue. 
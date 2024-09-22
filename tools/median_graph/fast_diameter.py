import networkx as nx
from multiprocessing import Pool, cpu_count
import pickle
from pathlib import Path
import argparse
from utils import build_graph
from cdl import CondorcetDomain
from functools import partial

# Parsing the arguments
parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=13)
parser.add_argument("-input_path", type=Path, default="../../")
args = parser.parse_args()
config = vars(args)
print(config)

# Function to calculate shortest paths from a single source node
def single_source_dijkstra_length(G, node):
    return nx.single_source_dijkstra_path_length(G, node)

# Function to calculate shortest paths for multiprocessing
def calculate_shortest_paths(G, node):
    return single_source_dijkstra_length(G, node)

# Function to calculate the diameter of the graph using multiple cores
def graph_diameter_parallel(G):
    # List of nodes in the graph
    nodes = list(G.nodes)
    
    # Number of available CPU cores
    num_cores = cpu_count()

    # Use functools.partial to fix G as a constant argument in the function
    calculate_paths_with_graph = partial(calculate_shortest_paths, G)

    # Use multiprocessing Pool to parallelize shortest path computation
    with Pool(num_cores) as pool:
        # Compute shortest paths from all nodes in parallel using partial function
        shortest_paths = pool.map(calculate_paths_with_graph, nodes)

    # Calculate the diameter: the maximum shortest path distance
    diameter = max(max(lengths.values()) for lengths in shortest_paths)
    return diameter

# Main script logic wrapped in the if __name__ == "__main__" block
if __name__ == "__main__":
    n = config['n']
    input_path = config['input_path'] / "python" / "properties"
    result_path = input_path / "results"
    result_path.mkdir(exist_ok=True, parents=True)
    result_file = result_path / "fast_diameter.txt"

    cd = CondorcetDomain(n)
    record_trss = input_path / "record_trss.pkl"
    
    with open(record_trss, 'rb') as f:
        trss = pickle.load(f)
        trs_list = trss[cd.n]
        for trs in trs_list:
            domain = cd.domain(trs)
            G = build_graph(cd, domain)
            # Compute diameter
            diameter = graph_diameter_parallel(G)
            with result_file.open('a') as f:
                f.write(f"n={n}, size={len(domain)}, diameter={diameter}\n")

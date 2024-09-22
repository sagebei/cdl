import networkx as nx
from multiprocessing import Pool, cpu_count
import pickle
from pathlib import Path
import argparse
from utils import build_graph
from cdl import CondorcetDomain

# Parsing the arguments
parser = argparse.ArgumentParser(description="get the result",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-n", type=int, default=8)
parser.add_argument("-input_path", type=Path, default="../../")
args = parser.parse_args()
config = vars(args)
print(config)

# Function to check if a set of nodes are reachable from a starting node
def check_reachability(G, nodes_chunk, start_node):
    # Use BFS to get the connected component containing the start_node
    connected_component = nx.node_connected_component(G, start_node)
    
    # Check if all nodes in the chunk are in the connected component
    return all(node in connected_component for node in nodes_chunk)

# Function to check graph connectivity in parallel
def is_connected_parallel(G):
    # List of nodes in the graph
    nodes = list(G.nodes)
    
    # Select a starting node
    start_node = nodes[0]
    
    # Number of CPU cores
    num_cores = cpu_count()
    
    # Split the nodes into chunks for parallel processing
    chunk_size = len(nodes) // num_cores + 1
    node_chunks = [nodes[i:i + chunk_size] for i in range(0, len(nodes), chunk_size)]
    
    # Use multiprocessing Pool to parallelize the reachability check
    with Pool(num_cores) as pool:
        # Each process checks if its chunk of nodes is reachable from the start node
        results = pool.starmap(check_reachability, [(G, chunk, start_node) for chunk in node_chunks])
    
    # If all processes return True, the graph is connected; otherwise, it is not
    return all(results)

# Example usage:
if __name__ == "__main__":
    n = config['n']
    input_path = config['input_path'] / "python" / "properties"
    result_path = input_path / "results"
    result_path.mkdir(exist_ok=True, parents=True)
    result_file = result_path / "fast_connected.txt"

    cd = CondorcetDomain(n)
    record_trss = input_path / "record_trss.pkl"
    
    with open(record_trss, 'rb') as f:
        trss = pickle.load(f)
        trs_list = trss[cd.n]
        for trs in trs_list:
            domain = cd.domain(trs)
            G = build_graph(cd, domain)
            size = len(domain)
            with result_file.open('a') as f:
                if is_connected_parallel(G):
                    f.write(f"n={n}, size={size}, connect={True}\n")
                else:
                    f.write(f"n={n}, size={size}, connect={False}\n")
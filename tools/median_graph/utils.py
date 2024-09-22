import networkx as nx

def check(state, non_1n3_3n1):
    non_1n3_3n2_count = 0
    for rule_id in state:

        if rule_id != 0:
            if rule_id != 1 and rule_id != 2:
                non_1n3_3n2_count += 1
    if non_1n3_3n2_count == non_1n3_3n1:
        return True

    return False

def count_rule(state):
    non_1n3_3n2_count = 0
    for rule_id in state:
        if rule_id != 0:
            if rule_id != 1 and rule_id != 2:
                non_1n3_3n2_count += 1
    return non_1n3_3n2_count

def core(cd, trs):
    domain = cd.domain(trs)
    size = len(domain)
    unique_iso_trss = set()
    for permutation in domain:
        iso_trs = cd.inverse_trs(trs, permutation, ["1N3", "3N1", "2N3", "2N1"])
        unique_iso_trss.add(tuple(cd.trs_to_state(iso_trs)))
    return size / len(unique_iso_trss)


def build_graph(cd, domain):
    G = nx.Graph()
    for p in domain:
        G.add_node(tuple(p))
    for p1 in domain:
        for p2 in domain:
            diff = [(idx, i, j) for idx, (i, j) in enumerate(zip(p1, p2)) if i != j]
            if len(diff) == 2:
                if (diff[0][0] - diff[1][0]) == -1:
                    if diff[0][1] == diff[1][2] and diff[0][2] == diff[1][1]:
                        G.add_edge(tuple(p1), tuple(p2))

    return G
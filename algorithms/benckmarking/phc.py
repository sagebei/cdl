import random
import itertools

def cutrules(Lsingle, k):
    Lred = []
    for v in Lsingle:
        if (v[0][2] == k):
            Lred.append([v[0], v[1]])
    return Lred


def stringext(str, k):
    str1 = []
    for j in str:
        for i in list(range(0, k)):
            B = list(j)
            B.insert(i, k)
            str1.append(B)
    return str1


def string123(Lsingle):
    rules3 = cutrules(Lsingle, 3)
    excp = []
    for r in rules3:
        if (r[1] == '1N3'):
            excp.append([2, 3, 1])
            excp.append([3, 2, 1])
        if (r[1] == '1N2'):
            excp.append([2, 1, 3])
            excp.append([3, 1, 2])
        if (r[1] == '2N1'):
            excp.append([2, 1, 3])
            excp.append([2, 3, 1])
        if (r[1] == '2N3'):
            excp.append([1, 3, 2])
            excp.append([3, 1, 2])
        if (r[1] == '3N1'):
            excp.append([3, 1, 2])
            excp.append([3, 2, 1])
        if (r[1] == '3N2'):
            excp.append([1, 3, 2])
            excp.append([2, 3, 1])
        if (r[1] == '1N1'):
            excp.append([1, 2, 3])
            excp.append([1, 3, 2])
        if (r[1] == '2N2'):
            excp.append([1, 2, 3])
            excp.append([3, 2, 1])
        if (r[1] == '3N3'):
            excp.append([1, 2, 3])
            excp.append([2, 1, 3])
    res = []
    for q in [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]:
        if not (q in excp):
            res.append(q)
    return res

def rules2strings(Lsingle, n):
    Wred = string123(cutrules(Lsingle, 3))
    for k in list(range(4, n + 1)):
        rulesk = cutrules(Lsingle, k)
        W = stringext(Wred, k)
        Wred = []
        for p in W:
            s = 0
            for v in rulesk:
                coor1 = p.index(v[0][0])
                coor2 = p.index(v[0][1])
                coor3 = p.index(v[0][2])
                if (v[1] == '1N2'):
                    if (coor2 < coor1) and (coor1 < coor3):
                        s = 1
                    if (coor3 < coor1) and (coor1 < coor2):
                        s = 1
                if (v[1] == '1N3'):
                    if (coor2 < coor3) and (coor3 < coor1):
                        s = 1
                    if (coor3 < coor2) and (coor2 < coor1):
                        s = 1

                if (v[1] == '2N1'):
                    if (coor2 < coor1) and (coor1 < coor3):
                        s = 1
                    if (coor2 < coor3) and (coor3 < coor1):
                        s = 1
                if (v[1] == '2N3'):
                    if (coor1 < coor3) and (coor3 < coor2):
                        s = 1
                    if (coor3 < coor1) and (coor1 < coor2):
                        s = 1
                if (v[1] == '3N1'):
                    if (coor3 < coor1) and (coor1 < coor2):
                        s = 1
                    if (coor3 < coor2) and (coor2 < coor1):
                        s = 1
                if (v[1] == '3N2'):
                    if (coor1 < coor3) and (coor3 < coor2):
                        s = 1
                    if (coor2 < coor3) and (coor3 < coor1):
                        s = 1
                if (v[1] == '1N1'):
                    if (coor1 < coor3) and (coor3 < coor2):
                        s = 1
                    if (coor1 < coor2) and (coor2 < coor3):
                        s = 1
                if (v[1] == '2N2'):
                    if (coor1 < coor2) and (coor2 < coor3):
                        s = 1
                    if (coor3 < coor2) and (coor2 < coor1):
                        s = 1
                if (v[1] == '3N3'):
                    if (coor1 < coor2) and (coor2 < coor3):
                        s = 1
                    if (coor2 < coor1) and (coor1 < coor3):
                        s = 1
            if (s == 0):
                Wred.append(p)
    return Wred


R = ['1N3', '2N3', '2N1', '3N1']


def create_random_function3(n):
    return {triple: random.randint(0, 3) for triple in itertools.combinations(range(1, n + 1), 3)}


def function2rules(f):
    out = []
    for value in f.keys():
        out.append((value, R[f[value]]))
    return out


def evaluate_function(f, n):
    rules = function2rules(f)
    return len(rules2strings(rules, n))


def get_neighbors(f, n):
    neighbors = []
    for triple in f.keys():
        for new_value in range(0, 4):
            if new_value != f[triple]:
                new_function = f.copy()
                new_function[triple] = new_value
                neighbors.append(new_function)
    return neighbors


def hill_climbing(n, N, restarts):
    best_function = create_random_function3(n)
    best_score = evaluate_function(best_function, n)

    for _ in range(restarts):
        current_function = create_random_function3(n)
        current_score = evaluate_function(current_function, n)

        for _ in range(N):
            neighbors = get_neighbors(current_function, n)
            neighbor_scores = [evaluate_function(neighbor, n) for neighbor in neighbors]

            best_neighbor_idx = max(range(len(neighbor_scores)), key=neighbor_scores.__getitem__)
            best_neighbor_score = neighbor_scores[best_neighbor_idx]

            if best_neighbor_score > current_score:
                current_score = best_neighbor_score
                current_function = neighbors[best_neighbor_idx]
            else:
                break

        if current_score > best_score:
            best_score = current_score
            best_function = current_function

    return best_function, best_score


def main():
    print(" n | Max Image Size | Best Function")
    print("====================================")

    N = 10000
    restarts = 2

    for n in range(4, 9):
        best_function, max_image_size = hill_climbing(n, N, restarts)
        print(f"{n:2d} | {max_image_size:13d} | {best_function}")


if __name__ == "__main__":
    main()



import sys
import greedy_insertion

def dist(i, j, node_loc):
    ret = (node_loc[i][0] - node_loc[j][0])**2
    ret += (node_loc[i][1] - node_loc[j][1])**2
    return ret**0.5

def get_closest_nodes(graph, N):
    closest_nodes = []
    for i in range(N):
        closest_nodes.append(sorted(graph[i].copy())[1:5])

    return closest_nodes

def calc_graph(node_loc, N):
    graph = []
    for i in range(N):
        graph.append([])
        for j in range(N):
            graph[-1].append(dist(i, j, node_loc))
    
    return graph


def init_insertion(inst_loc):
    ifile = open(inst_loc, 'r')
    lines = ifile.readlines()
    N = int(lines[2])

    node_loc = []
    for line in lines[3:]:
        loc = line.split()
        node_loc.append([float(loc[0]), float(loc[1])])
    ifile.close()

    graph = calc_graph(node_loc, N)

    closest_nodes = get_closest_nodes(graph, N)

    for i in range(N):
        for j in range(N):
            greedy_insertion(graph, [i, j], N, closest_nodes)

def main():
    # init_insertion(sys.argv[1])
    init_insertion("../tspd_instances/uniform/uniform-50-n9.txt")

if __name__ == "__main__":
    main()
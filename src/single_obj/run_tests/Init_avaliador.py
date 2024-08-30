# TSP tour:
# 0 8 2 1 7 4 6 3 5 
# Operations:
# 0,8,2|8,2,1|1,7,4|4,6,3|3,5,0|

# instance_loc = input()
# sol_loc = input()
import sys
from avaliador import avaliar

def dist(i, j, node_loc):
    ret = (node_loc[i][0] - node_loc[j][0])**2
    ret += (node_loc[i][1] - node_loc[j][1])**2
    return ret**0.5

def tsp_tour_cost(tour, node_loc):
    cost = 0
    for i in range(len(tour)-1):
        cost += dist(tour[i], tour[i+1], node_loc)
        # print(dist(tour[i], tour[i+1], node_loc))

    cost += dist(tour[0], tour[-1], node_loc)
    # print(dist(tour[0], tour[-1], node_loc))
    return cost

def init_avaliador(instance_loc, sol_loc):
    # print(instance_loc)
    # print(sol_loc)

    ifile = open(instance_loc, 'r')
    lines = ifile.readlines()
    alpha = float(lines[1])
    # print(f"1/alpha = {alpha}")
    N = int(lines[2])
    # print(f"N = {N}")

    node_loc = []
    for line in lines[3:]:
        loc = line.split()
        node_loc.append([float(loc[0]), float(loc[1])])
    # print(f"Localização dos nós:\n{node_loc}")
    if N != len(node_loc):
        print("DEU RUIM !!!!!\nNúmero de nós não correspondente")
    ifile.close()

    sfile = open(sol_loc, 'r')
    lines = sfile.readlines()
    tour = lines[-21].split()
    for i in range(len(tour)):
        tour[i] = int(tour[i])
    # print(f"Ciclo hamiltoniano: {tour}")
    best_solution = float(lines[-15].split()[-1])
    # print(f"best fstsp solution: {best_solution}")
    drone_path = lines[-19].split('|')[:-1]
    drone_ops = []
    for ops in drone_path:
        op = ops.split(',')
        drone_ops.append([int(op[0]), int(op[1]), int(op[2])])

    # print(f"drone operations:\n{drone_ops}")


    sfile.close()

    # print("tsp cost:",tsp_tour_cost(tour, node_loc))

    # input("press anything to continue...")

    return avaliar(best_solution, tour, drone_ops, node_loc, alpha, N, sol_loc)


def main():
    init_avaliador(sys.argv[1], sys.argv[2])
    # init_avaliador("../../../tspd_instances/uniform/uniform-50-n9.txt", "../solution.txt")

if __name__ == "__main__":
    main()
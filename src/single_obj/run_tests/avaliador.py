import math

debug = 2

def dist(i, j, node_loc):
    ret = (node_loc[i][0] - node_loc[j][0])**2
    ret += (node_loc[i][1] - node_loc[j][1])**2
    return ret**0.5

def dist_drone(i, j, k, node_loc, alpha):
    dist1 = (node_loc[i][0] - node_loc[j][0])**2
    dist1 += (node_loc[i][1] - node_loc[j][1])**2
    dist1 **= 0.5
    dist1 *= alpha

    dist2 = (node_loc[k][0] - node_loc[j][0])**2
    dist2 += (node_loc[k][1] - node_loc[j][1])**2
    dist2 **= 0.5
    dist2 *= alpha

    return dist1+dist2

def avaliar(cost, tour, drone_ops, node_locs, alpha, N, sol_loc):
    # print("solution location: " + sol_loc)
    if debug>1:
        print(alpha)

    if len(tour) == 0:
        print("Não há nós no tour")
        input("aperte enter para continuar...")
        return

    if tour[0] != 0:
        print("Tour não começa no 0")
        input("aperte enter para continuar...")
        return

    for i in range(N):
        if tour[i] >= N or tour[i] < 0:
            print("Nó não existente encontrado no tour", tour[i])
            input("aperte enter para continuar...")
            return

    tour_verify = [0] * N
    for i in range(N):
        tour_verify[tour[i]] += 1

    for ocurrence in tour_verify:
        if ocurrence != 1:
            print("Há nós repetidos ou faltando", ocurrence)
            input("aperte enter para continuar...")
            return

    if len(drone_ops) == 0:
        print("Não há operações com drone")

    for op in drone_ops:
        if len(op) != 3:
            print("Operação com menos ou mais de 3 nós", op)
            input("aperte enter para continuar...")
            return
    
    
    node_order = [0] * N
    for i in range(N):
        node_order[tour[i]] = i # salvando ordem dos nós
    if debug>1:
        print(f"node order: {node_order}")
        print(f"ciclo hamiltoniano: {tour}")
        print(f"operacoes do drone: {drone_ops}")

    for op in drone_ops:
        if op[1] != -1:
            if node_order[op[0]] >= node_order[op[1]] or (node_order[op[1]] >= node_order[op[2]] and op[2] != 0 and op == drone_ops[-1]):
                print("Operação fora de ordem", op)
                input("aperte enter para continuar...")
                return
        else:
            if node_order[op[0]] >= node_order[op[2]] and op[2] != 0 and op == drone_ops[-1]:
                print("Operação fora de ordem", op)
                input("aperte enter para continuar...")
                return

    for i in range(len(drone_ops)-1):
        if node_order[drone_ops[i][2]] > node_order[drone_ops[i+1][0]]:
            print("Operações fora de ordem", drone_ops[i], drone_ops[i+1])
            input("aperte enter para continuar...")
            return

    #Arrumar a partir daqui
    total_cost = 0

    truck_tour = tour + [N]

    node_locs = node_locs + [node_locs[0]]

    for op in drone_ops:
        if op[1] != -1:
            truck_tour.remove(op[1])

    truck_order = [0]*(N+1)

    for i in range(len(truck_tour)):
        truck_order[truck_tour[i]] = i


    for op in drone_ops:
        drone_cost = 0
        truck_cost = 0
        if debug>1:
            print("drone op:",op)

        if op[1] != -1:
            drone_cost = dist_drone(op[0], op[1], op[2], node_locs, alpha)
        
        if op[2] == 0:
            op[2] = N
    
        for i in range(truck_order[op[0]], truck_order[op[2]]):
            truck_cost += dist(truck_tour[i], truck_tour[i+1], node_locs)
            if debug>1:
                print("current truck op:",truck_tour[i], truck_tour[i+1])

        if debug>1:
            print("dronecost|truckcost:",drone_cost,"|", truck_cost)

        total_cost += max(drone_cost, truck_cost)


    if abs(total_cost - cost) >= 0:
        print("DIFERENÇA DE CUSTOS MUITO ALTA")
        print("custo total:",total_cost, "custo suprido:",cost)
        if debug:
            print("solution location: " + sol_loc)
            print(alpha)
        # input("aperte enter para continuar...")
    # else:
    #     print("Diferença aceitável")

    


import os


def levalor(texto, arquivo):
    pin = arquivo.find(texto)
    if pin == -1:
        return ""
    pfin = arquivo.find('\n', pin)
    tamanho = len(texto)
    return arquivo[pin+tamanho:pfin]


def levalorproxlinha(texto, arquivo):
    pin = arquivo.find(texto)
    if pin == -1:
        return ""
    pin = arquivo.find('\n', pin)
    pfin = arquivo.find('\n', pin+1)
    return arquivo[pin+1:pfin]


# split_types = [1, 2, 3, 4, 5, 6]
split_types = [-1]
# tsp_solution_types = ["nearest_neighbor"]
tsp_solution_types = ["lkh", "nearest_neighbor", "christofides",
                      "nearest_insertion", "cheapest_insertion"]

for split_type in split_types:
    for tsp_solution_type in tsp_solution_types:

        print(str(split_type) + " " + tsp_solution_type)

        # variáveis por definir
        # split_type = 0
        # tsp_solution_type = "lkh"
        csv_file_name = tsp_solution_type + \
            "_split_" + str(split_type) + ".csv"

        # ajuste para usar o find locality
        if split_type == -1:
            csv_file_name = tsp_solution_type + "_find_locality.csv"

        csv_file = open(csv_file_name, "w")

        instances_path = "../../../instancias/"
        tsp_solutions_path = "../../../tsp_solutions_" + tsp_solution_type + "/"
        fstsp_solutions_path = "../fstsp_solutions_" + tsp_solution_type + "_split_" + \
            str(split_type) + "/"

        # ajuste para usar o find locality
        if split_type == -1:
            fstsp_solutions_path = "../fstsp_solutions_" + \
                tsp_solution_type + "_find_locality/"

        doublecenter = "doublecenter"
        singlecenter = "singlecenter"
        uniform = "uniform"

        instances_type = [doublecenter, singlecenter, uniform]

        if split_type == -1:

            table_header = "Instance Name, Truck Speed, Drone Speed, Number of Nodes, TSP Sol Cost, Arcs in Gaux, Triples, Locality, Num Vert Loc > 1, Vertices labels / order / locality"
            # print(table_header)
            csv_file.write(table_header + "\n")

            for t in instances_type:
                instances_file_list = os.listdir(instances_path + t)
                # para ignorar pasta author_solutions
                # instances_file_list = instances_file_list[1:]
                instances_file_list.remove("author_solutions")

                for i in instances_file_list:
                    instance_path = instances_path + t + '/' + i
                    # print(instance_path)
                    tsp_solution_path = tsp_solutions_path + t + '/' + i
                    tsp_solution_path = tsp_solution_path[:-3] + "tspsol"
                    # print(tsp_solution_path)
                    fstsp_solution_path = fstsp_solutions_path + t + '/' + i
                    fstsp_solution_path = fstsp_solution_path[:-3] + "sol"
                    # print(hfstsp_solution_path)

                    # abrir aquivo .sol para pegar as informacoes
                    f = open(fstsp_solution_path, "r")
                    s = f.read()
                    # print(i, end=',')
                    truck_speed = levalor("Truck Speed:", s)
                    # print(truck_speed, end=',')
                    drone_speed = levalor("Drone Speed:", s)
                    # print(drone_speed, end=',')
                    num_nodes = levalor("Number of Nodes:", s)
                    # print(num_nodes, end=',')
                    TSP_sol_cost = levalor("TSP Sol Cost:", s)
                    # print(TSP_sol_cost, end=',')

                    Gaux_arcs = levalor(
                        "Number of arcs in the auxiliary graph:", s)
                    # print(Gaux_arcs, end=',')
                    triples = levalor("Number of triples considered:", s)
                    # print(triples, end=',')
                    locality = levalor(
                        "Locality:", s)
                    # print(locality, end=',')
                    numVertLoc = levalor(
                        "Number of vertices with locality > 1:", s)
                    # print(numVertLoc, end=',')

                    vertices_locality = levalorproxlinha(
                        "Vertices locality:", s)
                    vertices_locality = vertices_locality.split(' ')
                    # print(vertices_locality, end=',')

                    labels = levalorproxlinha(
                        "Vertices labels:", s)
                    labels = labels.split(' ')
                    # print(labels, end=',')
                    orders = levalorproxlinha(
                        "Vertices order:", s)
                    orders = orders.split(' ')
                    # print(orders, end=',')
                    comb = []
                    for j in range(int(numVertLoc)):
                        comb.append(labels[j] + ' / ' +
                                    orders[j] + ' / ' + vertices_locality[int(orders[j])])
                    comb = ', '.join(e for e in comb)
                    # print(comb)
                    csv_file.write(i + "," + truck_speed + "," + drone_speed + "," + num_nodes + "," + TSP_sol_cost +
                                   "," + Gaux_arcs + "," + triples + "," + locality + "," + numVertLoc + "," + comb + "\n")
                    f.close()

        else:

            table_header = "Instance Name, Truck Speed, Drone Speed, Number of Nodes, TSP Sol Cost, Arcs in Gaux, Triples, Gaux Ini Time, Gaux Building Time, Shortest Path Time, FSTSP Sol Cost, Average Time (ns)"
            print(table_header)
            csv_file.write(table_header + "\n")

            for t in instances_type:
                instances_file_list = os.listdir(instances_path + t)
                # para ignorar pasta author_solutions
                # instances_file_list = instances_file_list[1:]
                instances_file_list.remove("author_solutions")

                for i in instances_file_list:
                    instance_path = instances_path + t + '/' + i
                    # print(instance_path)
                    tsp_solution_path = tsp_solutions_path + t + '/' + i
                    tsp_solution_path = tsp_solution_path[:-3] + "tspsol"
                    # print(tsp_solution_path)
                    fstsp_solution_path = fstsp_solutions_path + t + '/' + i
                    fstsp_solution_path = fstsp_solution_path[:-3] + "sol"
                    # print(hfstsp_solution_path)

                    # abrir aquivo .sol para pegar as informacoes
                    f = open(fstsp_solution_path, "r")
                    s = f.read()
                    print(i, end=',')
                    truck_speed = levalor("Truck Speed:", s)
                    print(truck_speed, end=',')
                    drone_speed = levalor("Drone Speed:", s)
                    # print(drone_speed, end=',')
                    num_nodes = levalor("Number of Nodes:", s)
                    # print(num_nodes, end=',')
                    TSP_sol_cost = levalor("TSP Sol Cost:", s)
                    # print(TSP_sol_cost, end=',')

                    Gaux_arcs = levalor(
                        "Number of arcs in the auxiliary graph:", s)
                    # print(Gaux_arcs, end=',')
                    triples = levalor("Number of triples considered:", s)
                    # print(triples, end=',')
                    Gaux_ini_time = levalor(
                        "Time of initialization for auxiliary graph:", s)
                    # print(Gaux_ini_time, end=',')
                    Gaux_building_time = levalor(
                        "Time of building for auxiliary graph:", s)
                    # print(Gaux_building_time, end=',')
                    shortest_path_time = levalor(
                        "Time of shortest path algorithm:", s)
                    # print(shortest_path_time, end=',')
                    FSTSP_sol_cost = levalor("FSTSP Sol Cost:", s)
                    # print(FSTSP_sol_cost, end=',')
                    average_time_ns = levalor("Average Time (ns):", s)
                    # print(average_time_ns)
                    csv_file.write(i + "," + truck_speed + "," + drone_speed + "," + num_nodes + "," +
                                   TSP_sol_cost + "," + Gaux_arcs + "," + triples + "," +
                                   Gaux_ini_time + "," + Gaux_building_time + "," +
                                   shortest_path_time + "," + FSTSP_sol_cost + "," + average_time_ns + "\n")
                    f.close()

        csv_file.close()

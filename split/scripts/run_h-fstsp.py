import os


# split_types = [1, 2, 3, 4, 5, 6]
split_types = [-1]
# tsp_solution_types = ["nearest_neighbor"]
tsp_solution_types = ["nearest_neighbor", "nearest_insertion",
                      "cheapest_insertion", "christofides", "lkh"]


for split_type in split_types:
    for tsp_solution_type in tsp_solution_types:

        # variáveis por definir
        # split_type = 0
        # tsp_solution_type = "lkh"

        # exec_path = "..\\h-fstsp"
        exec_path = "..\\h-fstsp.exe"
        # exec_path = "../h-fstsp"

        instances_path = "../../../instancias/"
        tsp_solutions_path = "../../../tsp_solutions/" + tsp_solution_type + "/"
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

        if not os.path.exists(fstsp_solutions_path):
            os.mkdir(fstsp_solutions_path)

        for t in instances_type:
            instances_file_list = os.listdir(instances_path + t)
            # para ignorar pasta author_solutions
            # instances_file_list = instances_file_list[1:]
            instances_file_list.remove("author_solutions")

            print("Numero instancias " + t + " = " +
                  str(len(instances_file_list)))
            # print(instances_file_list)

            if not os.path.exists(fstsp_solutions_path + t):
                os.mkdir(fstsp_solutions_path + t)

            for i in instances_file_list:
                instance_path = instances_path + t + '/' + i
                # print(instance_path)
                tsp_solution_path = tsp_solutions_path + t + '/' + i
                tsp_solution_path = tsp_solution_path[:-3] + "tspsol"
                # print(tsp_solution_path)
                fstsp_solution_path = fstsp_solutions_path + t + '/' + i
                fstsp_solution_path = fstsp_solution_path[:-3] + "sol"
                # print(hfstsp_solution_path)

                exec_command = exec_path + " " + instance_path + " " + \
                    tsp_solution_path + " " + \
                    str(split_type) + " > " + fstsp_solution_path
                print(exec_command)
                # rodando com parâmetro 0 = nearest
                os.system(exec_command)

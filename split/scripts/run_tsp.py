import os
exec_path = "../tsp"

instances_path = "../../../instancias/"
tsp_solutions_path = "../../../tsp_solutions_cheapest_insertion/"

doublecenter = "doublecenter"
singlecenter = "singlecenter"
uniform = "uniform"

instances_type = [doublecenter, singlecenter, uniform]

if not os.path.exists(tsp_solutions_path):
    os.mkdir(tsp_solutions_path)


for t in instances_type:
    instances_file_list = os.listdir(instances_path + t)
    # para ignorar pasta author_solutions
    #instances_file_list = instances_file_list[1:]
    instances_file_list.remove("author_solutions")

    print("Numero instancias " + t + " = " + str(len(instances_file_list)))
    # print(instances_file_list)

    if not os.path.exists(tsp_solutions_path + t):
        os.mkdir(tsp_solutions_path + t)

    for i in instances_file_list:
        instance_path = instances_path + t + '/' + i
        # print(instance_path)
        solution_path = tsp_solutions_path + t + '/' + i
        solution_path = solution_path[:-3] + "tspsol"
        # print(solution_path)
        exec_command = exec_path + " " + instance_path + " 4 > " + solution_path
        print(exec_command)
        # rodando com parâmetro 0 = nearest
        os.system(exec_command)

import os
import sys


if len(sys.argv) == 2 and sys.argv[1] == "help":
    print("python3 run_brkga.py <max time> <num threads> <method>" + \
    "\n<max time>: maximum instance running time" + \
    "\n<num threads>: maximum threads for decoding in brkga")
    exit()

if len(sys.argv) < 3:
    print("python3 run_brkga.py <max time> <num threads>" + \
    "\nuse \"python3 run_brkga.py help\" for more details")
    exit()




exec_path = "../main_minimal"

instances_path = "../../instances/"
tsp_solutions_path = "../../fstsp_sol/"
runtime = sys.argv[1]
threads = sys.argv[2]
methods = [["no_initPop", 0], ["rand_initPop", 1], ["def_initPop", 2]]


if not os.path.exists(tsp_solutions_path):
    os.mkdir(tsp_solutions_path)


seeds_file = open("seeds.txt", "r")
seeds = seeds_file.readlines()

instances_file_list = os.listdir(instances_path)

print("Numero instancias = " + str(len(instances_file_list)) + \
      "\nNumero de seeds = " + str(len(seeds)))

for i in instances_file_list:    

    sol_by_inst_path = tsp_solutions_path + i[:-4]
    if not os.path.exists(sol_by_inst_path):
            os.mkdir(sol_by_inst_path)
    
    print("Rodando instancia " + i[:-4])
    for m in methods:
        sol_by_method_path = sol_by_inst_path + '/' + m[0]

        if not os.path.exists(sol_by_method_path):
            os.mkdir(sol_by_method_path)

        print("Usando metodo " + m[0])

        for seed in seeds:
            cleanseed = seed.strip()
            instance_path = instances_path + i
            # print(instance_path)
            
            solution_path = sol_by_method_path + '/' + cleanseed
            solution_path = solution_path + ".sol"
            # print(solution_path)
            exec_command = exec_path + " " + cleanseed + " ../config.conf " + runtime + \
            " " + threads + " " + str(m[1]) + " " + instance_path + " > " + solution_path
            print(exec_command)
            # rodando com parâmetro 0 = nearest
            os.system(exec_command)

seeds_file.close
import os
import sys
from multiprocessing import Pool

# python3 run_tsp_d.py  60 1

if len(sys.argv) == 2 and sys.argv[1] == "help":
    print("python3 run_brkga.py <max time> <num threads>" + \
    "\n<max time>: maximum instance running time" + \
    "\n<num threads>: maximum threads for decoding in brkga")
    exit()

if len(sys.argv) < 3:
    print("python3 run_brkga.py <max time> <num threads>" + \
    "\nuse \"python3 run_brkga.py help\" for more details")
    exit()



def execute(comando):
    print("Executing: " + str(comando))
    os.system(str(comando))

command_list = []
pool = Pool(processes=3)

exec_path = "../main_minimal_tsp"

instances_path = "../../../tspd_instances/"
tsp_solutions_path = "../../../tsp_sol/"
runtime = sys.argv[1]
threads = sys.argv[2]
methods = [["no_initPop", 0], ["rand_initPop", 1], ["def_initPop", 2]]


if not os.path.exists(tsp_solutions_path):
    os.mkdir(tsp_solutions_path)


seeds_file = open("seeds.txt", "r")
seeds = seeds_file.readlines()

instances_folders = os.listdir(instances_path)


for f in instances_folders:
    instances_folder_especific = instances_path + f + '/' 
    instances_file_list = os.listdir(instances_folder_especific)
    instances_file_list.remove("solutions")

    sol_by_folder_path = tsp_solutions_path + f
    if not os.path.exists(sol_by_folder_path):
        os.mkdir(sol_by_folder_path)
    for i in instances_file_list:    
        instance_path = instances_folder_especific + i
        num_nodes = instance_path.split('-')
        if (int(num_nodes[-1][1:-4])==100 or int(num_nodes[-1][1:-4])==50) and num_nodes[1][:5]!="alpha":
            sol_by_inst_path = sol_by_folder_path + '/' + i[:-4]
            if not os.path.exists(sol_by_inst_path):
                    os.mkdir(sol_by_inst_path)
            
            for m in methods[1:-1]:
                sol_by_method_path = sol_by_inst_path + '/' + m[0]
                
                if not os.path.exists(sol_by_method_path):
                    os.mkdir(sol_by_method_path)

                for seed in seeds:
                    cleanseed = seed.strip()
                    # print(instance_path)
                    
                    solution_path = sol_by_method_path + '/' + cleanseed
                    solution_path = solution_path + ".sol"

                    
                    realruntime = runtime if int(num_nodes[-1][1:-4]) > 30 else '5'
                    # print(solution_path)
                    exec_command = exec_path + " " + cleanseed + " ../config-basic.conf " + realruntime + \
                    " " + threads + " " + str(m[1]) + " " + instance_path + " > " + solution_path
                    command_list.append(exec_command)
                    
                    if len(command_list) == 3:
                        pool.map(execute, command_list)
                        command_list.clear()

seeds_file.close()

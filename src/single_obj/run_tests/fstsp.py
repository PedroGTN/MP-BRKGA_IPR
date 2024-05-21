import os
import sys
from multiprocessing import Pool

# python3 run_tsp_d.py  60 1

if len(sys.argv) == 2 and sys.argv[1] == "help":
    print("python3 run_brkga.py <num threads> <suffix>" + \
    "\n<num threads>: maximum threads for decoding in brkga" + \
    "\n<suffix>: to be added to the end of the filenames")
    exit()

if len(sys.argv) < 3:
    print("python3 run_brkga.py <num threads> <suffix>" + \
    "\nuse \"python3 run_brkga.py help\" for more details")
    exit()



def execute(comando):
    print("Executing: " + str(comando))
    os.system(str(comando))


exec_path = "../main_minimal"
suffix = sys.argv[2]
instances_path = "../../../tspd_instances/"
tsp_solutions_path = "../../../fstsp_sol_" + suffix + '/'
runtimes = [15, 30, 60, 120, 240]
threads = int(sys.argv[1])
methods = [["no_initPop", 0], ["rand_initPop", 1], ["def_initPop", 2]]

command_list = []
pool = Pool(processes=threads)

if not os.path.exists(tsp_solutions_path):
    os.mkdir(tsp_solutions_path)


seeds_file = open("seeds.txt", "r")
seeds = seeds_file.readlines()

instances_folders = os.listdir(instances_path)


for t in runtimes:
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
            if (int(num_nodes[-1][1:-4])==100 or int(num_nodes[-1][1:-4])==50 or int(num_nodes[-1][1:-4])==75):
                sol_by_inst_path = sol_by_folder_path + '/' + i[:-4]
                if not os.path.exists(sol_by_inst_path):
                        os.mkdir(sol_by_inst_path)
                
                for m in methods[1:-1]:
                    sol_by_method_path = sol_by_inst_path + '/' + m[0]

                    if not os.path.exists(sol_by_method_path):
                        os.mkdir(sol_by_method_path)

                    sol_by_runtime = sol_by_method_path + '/' + str(t)
                    
                    if not os.path.exists(sol_by_runtime):
                        os.mkdir(sol_by_runtime)

                    for seed in seeds:
                        cleanseed = seed.strip()
                        # print(instance_path)
                        
                        solution_path = sol_by_runtime + '/' + cleanseed + ".sol"
                        
                        realruntime = str(t) if int(num_nodes[-1][1:-4]) > 30 else '5'
                        # print(solution_path)
                        exec_command = exec_path + " " + cleanseed + " ../config-basic.conf " + realruntime + \
                        " 1 " + str(m[1]) + " " + instance_path + " > " + solution_path + '&& rm ../*concorde_*'

                        if(not os.path.isfile(solution_path)):
                            command_list.append(exec_command)
                        
                        if len(command_list) == threads:
                            pool.map(execute, command_list)
                            command_list.clear()

seeds_file.close()


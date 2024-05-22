import os
import sys

exec_path = "../main_generate_tours"
instances_path = "../../../tspd_instances/"
h_solutions_path = "../../../bolhas_elipticas_sol/"


if not os.path.exists(h_solutions_path):
    os.mkdir(h_solutions_path)

instances_folders = os.listdir(instances_path)

for f in instances_folders:
        instances_folder_especific = instances_path + f + '/' 
        instances_file_list = os.listdir(instances_folder_especific)
        instances_file_list.remove("solutions")

        sol_by_folder_path = h_solutions_path + f
        if not os.path.exists(sol_by_folder_path):
            os.mkdir(sol_by_folder_path)
        for i in instances_file_list:
            instance_path = instances_folder_especific + i
            num_nodes = instance_path.split('-')
            if (int(num_nodes[-1][1:-4])==100 or int(num_nodes[-1][1:-4])==50 or int(num_nodes[-1][1:-4])==75):
                solution_path = sol_by_folder_path + '/' + i[:-4] + '.sol'

                comando = exec_path + " " + instance_path + " > " + solution_path + '&& rm *concorde_*'
                print("Executing: " + str(comando))
                os.system(str(comando))
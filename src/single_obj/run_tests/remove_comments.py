import os
import re

#Colocar folder das instâncias aqui:
instaces_folder = "../../../tspd_instances/"

os.chdir(instaces_folder)

folder_list = os.listdir()



for f in folder_list:
    sol_folder = f + "/solutions/"
    sol_list = os.listdir(sol_folder)
    for i in sol_list:
        aux_file = open("../aux.txt", 'w')
        file_name = sol_folder + i
        with open(file_name) as sol_file:
            for s in sol_file:
                sm = re.sub('([a-z])\w+|(\/\*.*?\*\/)', '', s)
                aux_file.write(sm)
        aux_file.close()
        sol_file = open(file_name, 'w')
        with open("../aux.txt") as aux:
            for a in aux:
                if not a.isspace():
                    sol_file.write(a)

    folder_name = f + "/"
    instaces_list = os.listdir(folder_name)
    instaces_list.remove("solutions")
    for i in instaces_list:
        aux_file = open("../aux.txt", 'w')
        file_name = folder_name + i
        with open(file_name) as inst_file:
            for s in inst_file:
                sm = re.sub('([a-z])\w+|(\/\*.*?\*\/)', '', s)
                aux_file.write(sm)
        aux_file.close()
        inst_file = open(file_name, 'w')
        with open("../aux.txt") as aux:
            for a in aux:
                if not a.isspace():
                    inst_file.write(a)
                

        
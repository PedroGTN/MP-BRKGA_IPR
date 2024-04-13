import os
from opt_sol import opt_sol
from sol2tikz import sol2tikz

#nome inst, num nodes, double/single/uniform, best_fitness, current_iteration, last_update_iteration, last_update_time, stalled_iterations, largest_iteration_offset

os.chdir("../../../fstsp_sol/")
sol_path = "../tspd_instances/"
tikz_path = "../tikz_files/"

if not os.path.exists(tikz_path):
    os.mkdir(tikz_path)

csv_file_name = "table.csv"

csv_file = open(csv_file_name, 'w')

sol_file_list = os.listdir()
sol_file_list.remove(csv_file_name)

csv_file.write("instance_name,num_nodes,instance_type,optimal_solution,best_fitness,opt_to_fit_dist,opt_to_fit_percent,current_iteration,last_update_iteration,current_time,last_update_time,stalled_iterations,largest_iteration_offset\n")

num_list = [50, 100]

for inst_type in sol_file_list:
    inst_list = os.listdir(inst_type + '/')
    tikz_type_loc = tikz_path + inst_type + '/'
    if not os.path.exists(tikz_type_loc):
        os.mkdir(tikz_type_loc)
    
    #print(inst_type)

    for num in num_list:
        #print(num)
        for i in inst_list:
            #print(i)
            sol_file_loc = sol_path + inst_type + '/solutions/' + i + '-tsp.txt'

            tikz_inst_loc = tikz_type_loc + i + '/'
            if not os.path.exists(tikz_inst_loc):
                os.mkdir(tikz_inst_loc)

            instance_loc = sol_path + inst_type + '/' + i + '.txt'            
            num_nodes = i.split('-')
            if(int(num_nodes[-1][1:]) == num):
                csv_file.write(i + ',' + num_nodes[-1][1:] + ',' + inst_type + ',')
                inst_folder = inst_type + '/' + i
                mode_list = os.listdir(inst_folder)
                optimal_solution = opt_sol(instance_loc, sol_file_loc)
                best_fitness = 0.0
                current_iteration = 0.0
                last_update_iteration = 0.0
                last_update_time = 0.0
                current_time = 0.0
                stalled_iterations = 0.0
                largest_iteration_offset = 0.0

                for m in mode_list:
                    tikz_mode_loc = tikz_inst_loc + m + '/'
                    if not os.path.exists(tikz_mode_loc):
                        os.mkdir(tikz_mode_loc)

                    mode_folder = inst_folder + '/' + m
                    seed_list = os.listdir(mode_folder)
                    for s in seed_list:
                        tikz_seed_loc = tikz_mode_loc + s + '/'
                        if not os.path.exists(tikz_seed_loc):
                            os.mkdir(tikz_seed_loc)
                            
                        sol2tikz(mode_folder+'/'+s, tikz_seed_loc+'path.tex', instance_loc)

                        sfile = open(mode_folder + '/' + s, 'r')
                        lines = sfile.readlines()
                        best_fitness += float(lines[-15].split()[-1])
                        current_iteration += float(lines[-14].split()[-1])
                        last_update_iteration += float(lines[-13].split()[-1])
                        last_update_time += float(lines[-11].split()[-1][:-1])
                        current_time += float(lines[-12].split()[-1][:-1])
                        stalled_iterations += float(lines[-9].split()[-1])
                        largest_iteration_offset += float(lines[-10].split()[-1])




                
                csv_file.write( ("{:.4f}".format(optimal_solution)) + ',' + str(best_fitness/5) + ',' + ("{:.4f}".format((best_fitness/5) - optimal_solution)) + ',' + ("{:.4f}".format(((best_fitness/5)/optimal_solution)*100)) + '%,' + str(current_iteration/5) + ',' + str(last_update_iteration/5) + ',' + str(current_time/5) + ','+ str(last_update_time/5) + ',' + str(stalled_iterations/5) + ',' + str(largest_iteration_offset/5) + "\n")
        
            


print("done")
csv_file.close()
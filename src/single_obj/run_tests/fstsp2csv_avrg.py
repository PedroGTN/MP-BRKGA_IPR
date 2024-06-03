import os
from opt_sol import opt_sol
from sol2tikz import sol2tikz
import sys

#nome inst, num nodes, double/single/uniform, best_fitness, current_iteration, last_update_iteration, last_update_time, stalled_iterations, largest_iteration_offset

if len(sys.argv) < 2:
    input = ""
else:
    input = sys.argv[1]


os.chdir("../../../fstsp_sol_" + input + '/')
sol_path = "../tspd_instances/"
tikz_path = "../fstsp_" + input + "_tikz_files/"

if not os.path.exists(tikz_path):
    os.mkdir(tikz_path)

csv_file_name = "table_" + input + "_avrg.csv"

csv_file = open(csv_file_name, 'w')

sol_file_list = os.listdir()
sol_file_list.remove(csv_file_name)

csv_file.write("num_nodes,instance_type,alpha,runtime,optimal_tsp_solution,best_fitness,opt_to_fit_dist,opt_to_fit_percent,current_iteration,last_update_iteration,current_time,last_update_time,stalled_iterations,largest_iteration_offset\n\n")

num_list = [50, 75, 100]
alphas = [1, 2, 3]
runtimes = [15, 30, 60, 120]

for inst_type in sol_file_list:
    inst_list = os.listdir(inst_type + '/')
    tikz_type_loc = tikz_path + inst_type + '/'
    if not os.path.exists(tikz_type_loc):
        os.mkdir(tikz_type_loc)
    
    #print(inst_type)

    for num in num_list:
        # print(num)
        for alpha in alphas:
            for rtime in runtimes:
                
                csv_file.write(str(num) + ',' + inst_type + ',' + str(alpha) + ',' + str(rtime) + ',')
                optimal_solution = 0.0
                best_fitness = 0.0
                current_iteration = 0.0
                last_update_iteration = 0.0
                last_update_time = 0.0
                current_time = 0.0
                stalled_iterations = 0.0
                largest_iteration_offset = 0.0
                media = 0
                media_tsp = 0

                for inst in inst_list:
                    inst_split = inst.split('-')

                    loc_alpha = 2
                    if(inst_split[1][:-2] == 'alpha'):
                            loc_alpha = int(inst_split[1][-1])

                    if( loc_alpha != alpha or int(inst_split[-1][1:]) != num):
                        continue
                    #print(i)
                    
                    sol_file_loc = sol_path + inst_type + '/solutions/' + inst + '-tsp.txt'
                    tikz_inst_loc = tikz_type_loc + inst + '/'
                    if not os.path.exists(tikz_inst_loc):
                        os.mkdir(tikz_inst_loc)

                    instance_loc = sol_path + inst_type + '/' + inst + '.txt'            
                    inst_folder = inst_type + '/' + inst
                    mode_list = os.listdir(inst_folder)

                    for m in mode_list:
                        tikz_mode_loc = tikz_inst_loc + m + '/'
                        if not os.path.exists(tikz_mode_loc):
                            os.mkdir(tikz_mode_loc)

                        mode_folder = inst_folder + '/' + m

                        runtime_list = os.listdir(mode_folder)
                        for t in runtime_list:
                            
                            if(int(t) != rtime):
                                continue

                            optimal_solution += opt_sol(instance_loc, sol_file_loc)
                            media_tsp += 1

                            tikz_runtime_loc = tikz_mode_loc + t + '/'
                            if not os.path.exists(tikz_runtime_loc):
                                os.mkdir(tikz_runtime_loc)

                            runtime_folder = mode_folder + '/' + t

                            # mudar isso quando for rodar nos novos testes de mode_folder pra runtime_folder
                            seed_list = os.listdir(runtime_folder)

                            for s in seed_list:
                                tikz_seed_loc = tikz_runtime_loc + s + '/'
                                if not os.path.exists(tikz_seed_loc):
                                    os.mkdir(tikz_seed_loc)

                                # mudar isso aqui também
                                seed_folder = runtime_folder + '/' + s
                                    
                                sol2tikz(seed_folder, tikz_seed_loc+'path.tex', instance_loc)

                                sfile = open(seed_folder, 'r')
                                lines = sfile.readlines()
                                best_fitness += float(lines[-15].split()[-1])
                                current_iteration += float(lines[-14].split()[-1])
                                last_update_iteration += float(lines[-13].split()[-1])
                                last_update_time += float(lines[-11].split()[-1][:-1])
                                current_time += float(lines[-12].split()[-1][:-1])
                                stalled_iterations += float(lines[-9].split()[-1])
                                largest_iteration_offset += float(lines[-10].split()[-1])
                                media += 1

                csv_file.write( ("{:.4f}".format(optimal_solution/media_tsp)) + ',' + ("{:.4f}".format(best_fitness/media)) + ',' + ("{:.4f}".format((best_fitness/media) - optimal_solution/media_tsp)) + ',' + ("{:.4f}".format((((best_fitness/media)-(optimal_solution/media_tsp))/(optimal_solution/media_tsp))*100)) + '%,' + ("{:.4f}".format(current_iteration/media)) + ',' + ("{:.4f}".format(last_update_iteration/media)) + ',' + ("{:.4f}".format(current_time/media)) + ','+ ("{:.4f}".format(last_update_time/media)) + ',' + ("{:.4f}".format(stalled_iterations/media)) + ',' + ("{:.4f}".format(largest_iteration_offset/media)) + "\n")
                if(rtime == runtimes[-1]):
                    csv_file.write('\n')
                    if(alpha == 3):
                      csv_file.write('\n')

        
            


print("done")
csv_file.close()
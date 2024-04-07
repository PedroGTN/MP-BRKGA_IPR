import os

#nome inst, num nodes, double/single/uniform, best_fitness, current_iteration, last_update_iteration, last_update_time, stalled_iterations, largest_iteration_offset

os.chdir("../../../fstsp_sol/")

csv_file_name = "table.csv"

csv_file = open(csv_file_name, 'w')

sol_file_list = os.listdir()
sol_file_list.remove(csv_file_name)

csv_file.write("instance_name,num_nodes,instance_type,best_fitness,current_iteration,last_update_iteration,last_update_time,stalled_iterations,largest_iteration_offset\n")

num_list = [50, 100]

for inst_type in sol_file_list:
    inst_list = os.listdir(inst_type + '/')
    
    for num in num_list:
        for i in inst_list:
            num_nodes = i.split('-')
            if(int(num_nodes[-1][1:]) == num):
                csv_file.write(i + ',' + num_nodes[-1][1:] + ',' + inst_type + ',')
                inst_folder = inst_type + '/' + i
                mode_list = os.listdir(inst_folder)
                best_fitness = 0.0
                current_iteration = 0.0
                last_update_iteration = 0.0
                last_update_time = 0.0
                stalled_iterations = 0.0
                largest_iteration_offset = 0.0

                for m in mode_list:
                    mode_folder = inst_folder + '/' + m
                    seed_list = os.listdir(mode_folder)
                    for s in seed_list:
                        sfile = open(mode_folder + '/' + s, 'r')
                        lines = sfile.readlines()
                        best_fitness += float(lines[-15].split()[-1])
                        current_iteration += float(lines[-14].split()[-1])
                        last_update_iteration += float(lines[-13].split()[-1])
                        last_update_time += float(lines[-11].split()[-1][:-1])
                        stalled_iterations += float(lines[-9].split()[-1])
                        largest_iteration_offset += float(lines[-10].split()[-1])




                
                csv_file.write(str(best_fitness/5) + ',' + str(current_iteration/5) + ',' + str(last_update_iteration/5) + ',' + str(last_update_time/5) + ',' + str(stalled_iterations/5) + ',' + str(largest_iteration_offset/5) + "\n")
        
            


print("done")
csv_file.close()
import os
import sys

h_solutions_path = "../../../bolhas_elipticas_sol/"
tsp_solutions_path = "../../../concorde/"

if not os.path.exists(tsp_solutions_path):
    os.mkdir(tsp_solutions_path)

csv_file_name = h_solutions_path + 'table_BE.csv'
csv_avg_file_name = h_solutions_path + 'table_BE_avg.csv'


# node_list = [5, 6, 7, 8, 9, 10, 20, 50, 75, 100, 175, 250, 375, 500]
node_list = [50, 75, 100]
alpha_list = [1, 2, 3]
num_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 0]

solution_type = os.listdir(h_solutions_path)

solution_type.remove('table_BE.csv')
solution_type.remove('table_BE_avg.csv')

with open(csv_file_name, 'w') as csv, open (csv_avg_file_name, 'w') as csv_avg:
    csv_avg.write('num_nodes,instance_type,alpha,total_runtime,optimal_tsp_solution,optimal_tsp_split,lkh_cost,lkh_split,best_solution_cost,opt_to_best_dist,opt_to_best_percent,optsplit_to_best_dist,optsplit_to_best_percent\n')
    csv.write('instance_name,num_nodes,instance_type,alpha,total_runtime,optimal_tsp_solution,optimal_tsp_split,lkh_cost,lkh_split,best_solution_name,best_solution_cost,opt_to_best_dist,opt_to_best_percent,optsplit_to_best_dist,optsplit_to_best_percent\n')
    for n in node_list:
        for a in alpha_list:
            for t in solution_type:
                    
                count = 10
                total_runtime_avg = 0.0
                opt_tsp_avg = 0.0
                opt_tsp_split_avg = 0.0
                best_sol_avg = 0.0
                lkh_split_avg = 0.0
                lkh_avg = 0.0

                tsp_sol_by_type = tsp_solutions_path + t + '/'

                if not os.path.exists(tsp_sol_by_type):
                    os.mkdir(tsp_sol_by_type)

                solutions = os.listdir(h_solutions_path + t)
                for i in num_list:
                    for s in solutions:
                        sol_path = h_solutions_path + t + '/' + s
                        tsp_solution = tsp_sol_by_type + s[:-3] + 'tspsol' 


                        if(int(s.split('-')[-1][1:-4]) != n):
                            continue

                        alpha = 2
                        if(s.split('-')[1][:-2] == 'alpha'):
                            alpha = int(s.split('-')[1][-1])

                        if(alpha != a):
                            continue

                        if(alpha == 2):
                            if(int(s.split('-')[1][-1]) != i):
                                continue
                        else:
                            if(int(s.split('-')[2][-1]) != i):
                                continue

                        with open(sol_path) as sol, open(tsp_solution, 'w') as tspsol:

                            sol_lines = sol.readlines()
                            tspsol.write(sol_lines[9].split(':')[2][1:])
                            csv.write(s[:-4] + ',' + str(n) + ',' + t + ',' + str(alpha) + ',')
                            total_runtime = float(sol_lines[0].split(':')[1].strip()) + float(sol_lines[1].split(':')[1].strip()) + float(sol_lines[2].split(':')[1].strip()) + float(sol_lines[3].split(':')[1].strip())
                            opt_tsp = float(sol_lines[5].split(':')[2].strip())
                            opt_tsp_split = float(sol_lines[11].split(':')[2].strip())
                            csv.write(("{:.4f}".format(total_runtime)) + ',' + str(opt_tsp) + ',' + str(opt_tsp_split) + ',')
                            csv.write(("{:.4f},".format(float(sol_lines[6].split(':')[2].strip()))))
                            csv.write(("{:.4f},".format(float(sol_lines[14].split(':')[2].strip()))))
                            best_sol = float(sol_lines[8].split(':')[1].strip())
                            csv.write(sol_lines[7].split(':')[1].strip() + ',' + str(best_sol) + ',')
                            csv.write(("{:.4f}".format(best_sol - opt_tsp)) + ',')
                            csv.write(("{:.4f}%".format(100 * best_sol/opt_tsp)) + ',')
                            csv.write(("{:.4f}".format(best_sol - opt_tsp_split)) + ',')
                            csv.write(("{:.4f}%".format(100 * best_sol/opt_tsp_split)) + '\n')
                            total_runtime_avg += total_runtime
                            opt_tsp_avg += opt_tsp
                            opt_tsp_split_avg += opt_tsp_split
                            best_sol_avg += best_sol
                            lkh_split_avg += float(sol_lines[14].split(':')[2].strip())
                            lkh_avg += float(sol_lines[6].split(':')[2].strip())
                    
                csv_avg.write(str(n) + ',' + t + ',' + str(a) + ',')
                csv_avg.write(("{:.4f},".format(total_runtime_avg/count)) + ("{:.4f},".format(opt_tsp_avg/count)) + ("{:.4f},".format(opt_tsp_split_avg/count)))
                csv_avg.write(("{:.4f},".format(lkh_avg/count)) + ("{:.4f},".format(lkh_split_avg/count)))
                csv_avg.write(("{:.4f},".format(best_sol_avg/count)) + ("{:.4f},".format(best_sol_avg/count - opt_tsp_avg/count)) + ("{:.4f}%,".format(100 * (best_sol_avg/count)/(opt_tsp_avg/count))))
                csv_avg.write(("{:.4f},".format(best_sol_avg/count - opt_tsp_split_avg/count)) + ("{:.4f}%\n".format(100* (best_sol_avg/count)/(opt_tsp_split_avg/count))))



print('done')
            

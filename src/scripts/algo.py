import sys
import os
import math as m

def dist_calc(n1, n2, locations):
    node1 = locations[n1].split()
    node2 = locations[n2].split()
    sum = pow(float(node1[0]) - float(node2[0]), 2)
    sum += pow(float(node1[1]) - float(node2[1]), 2)
    #print(m.sqrt(sum))
    return m.sqrt(sum)


def opt_sol(instance, sol):
    sol_file = open(sol)
    inst_file = open(instance)
    ret = 0.0
    operations = sol_file.readlines()[0].split()
    locations = inst_file.readlines()[3:]

    ret+=dist_calc(int(operations[0]), int(operations[-1]), locations)
    num_no = 1

    num = len(operations)

    for i in range (0, num):
        for j in range (i+1, num):
            if(operations[i] == operations[j]):
                print(operations[i], operations[j], i, j)
                print("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
                exit()

    for i in range(len(operations)-1):
        ret += dist_calc(int(operations[i]), int(operations[i+1]), locations)
        num_no += 1

    return [ret, num_no]


concorde_folder = '../../../concorde/'
instances_path = "../../../tspd_instances/"

instance_type = os.listdir(instances_path)


with open('algo.csv', 'w') as csv:
    csv.write("Instance Name, Drone Speed, Number of Nodes, Instance Type, TSP Sol Cost, Number of Nodes Visited\n")
    for t in instance_type:
        instance_list = os.listdir(instances_path + t)
        instance_list.remove('solutions')

        for inst in instance_list:
            inst_path = instances_path + t + '/' + inst
            conc_path = concorde_folder + t + '/' + inst[:-3] + 'tspsol'
            with open(inst_path) as sol:
                lines = sol.readlines()
                csv.write(inst + ', ' + lines[1].strip() + ', ' + lines[2].strip() + ', ' + inst.split('-')[0] + ', ')
                ret = opt_sol(inst_path, conc_path)
                csv.write(str(ret[0]) + ', ' + str(ret[-1]) + '\n')

print('done')
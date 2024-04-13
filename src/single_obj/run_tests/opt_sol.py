import math as m

def opt_sol(instance, sol):
    sol_file = open(sol)
    inst_file = open(instance)
    ret = 0.0
    operations = sol_file.readlines()
    locations = inst_file.readlines()[3:]

    for op in operations[1:]:
        n = op.split()
        ret+=dist_calc(int(n[0]), int(n[1]), locations)

    print(ret)
    return ret

def dist_calc(n1, n2, locations):
    node1 = locations[n1].split()
    node2 = locations[n2].split()
    sum = pow(float(node1[0]) - float(node2[0]), 2)
    sum += pow(float(node1[1]) - float(node2[1]), 2)
    #print(m.sqrt(sum))
    return m.sqrt(sum)

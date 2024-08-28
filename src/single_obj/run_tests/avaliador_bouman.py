import os
import sys
from avaliador import avaliar

inst_dir = "../../../tspd_instances"

inst_type = os.listdir(inst_dir)


def avaliar_all():
    for type in inst_type:
        sol_folder = inst_dir + '/' + type + '/' + 'solutions' + '/'
        solutions = os.listdir(sol_folder)
        
        i = 0
        while i < len(sol_folder):
            if solutions[i].split('-')[-1] == 'tsp.txt':
                solutions.pop(i)
            else:
                i = i+1
                
        for sol in solutions:
            drone_ops = []
            tour = []
            instance_loc = inst_dir + '/' + type + '/' + sol[:-7] + '.txt'
            ifile = open(instance_loc, 'r')
            lines = ifile.readlines()
            alpha = float(lines[1])
            N = int(lines[2])

            node_loc = []
            for line in lines[3:]:
                loc = line.split()
                node_loc.append([float(loc[0]), float(loc[1])])
            if N != len(node_loc):
                print("DEU RUIM !!!!!\nNúmero de nós não correspondente")
            ifile.close()


            print(sol)
            sol_file = sol_folder + sol
            file = open(sol_file)
            lines = file.readlines()[1:]
            for line in lines:
                lsplit = line.split()
                if not int(lsplit[0]) in tour:
                    tour.append(int(lsplit[0]))

                drone_ops.append([int(lsplit[0]), int(lsplit[2]), int(lsplit[1])])
                
                if drone_ops[-1][0] == 0 and drone_ops[-1][1] == -1 and drone_ops[-1][2] == 0:
                    drone_ops.pop(-1)

                tour_node_num = -(int(lsplit[3]))

                if tour_node_num:
                    for node in lsplit[tour_node_num:]:
                        tour.append(int(node))
                if int(lsplit[2]) != -1:
                    tour.append(int(lsplit[2]))

                if not int(lsplit[1]) in tour:
                    tour.append(int(lsplit[1]))
            input("aperte para continuar...")

            print('Drone_ops:',drone_ops, '\nTour:', tour)

            avaliar(-1, tour, drone_ops, node_loc, alpha, N, sol_file)


def avaliar_bouman(sol_file):
    sol_split = sol_file.split('/')
    sol = sol_split[-1]
    type = sol_split[-3]
    drone_ops = []
    tour = []
    instance_loc = inst_dir + '/' + type + '/' + sol[:-7] + '.txt'
    ifile = open(instance_loc, 'r')
    lines = ifile.readlines()
    alpha = float(lines[1])
    N = int(lines[2])

    node_loc = []
    for line in lines[3:]:
        loc = line.split()
        node_loc.append([float(loc[0]), float(loc[1])])
    if N != len(node_loc):
        print("DEU RUIM !!!!!\nNúmero de nós não correspondente")
    ifile.close()

    print(sol)
    file = open(sol_file)
    lines = file.readlines()[1:]
    for line in lines:
        lsplit = line.split()
        if not int(lsplit[0]) in tour:
            tour.append(int(lsplit[0]))

        drone_ops.append([int(lsplit[0]), int(lsplit[2]), int(lsplit[1])])
        
        if drone_ops[-1][0] == 0 and drone_ops[-1][1] == -1 and drone_ops[-1][2] == 0:
            drone_ops.pop(-1)

        tour_node_num = -(int(lsplit[3]))

        if tour_node_num:
            for node in lsplit[tour_node_num:]:
                tour.append(int(node))
        if int(lsplit[2]) != -1:
            tour.append(int(lsplit[2]))

        if not int(lsplit[1]) in tour:
            tour.append(int(lsplit[1]))
    input("aperte para continuar...")

    print('Drone_ops:',drone_ops, '\nTour:', tour)

    avaliar(-1, tour, drone_ops, node_loc, alpha, N, sol_file)


def main():
    if sys.argv[1] == 'all':
        avaliar_all()
    else:
        avaliar_bouman(sys.argv[1])

if __name__ == "__main__":
    main()
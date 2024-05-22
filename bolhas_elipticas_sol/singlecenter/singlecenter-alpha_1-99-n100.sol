WARNING: Large edge lengths in sparse graph
Default Edge Length: 8388607
lk tour: [ 0 90 21 65 45 74 78 15 16 56 46 20 1 19 94 75 72 42 23 97 68 61 25 98 31 86 6 12 87 2 60 70 85 44 38 10 99 80 88 51 43 71 32 50 47 79 76 67 84 22 8 7 58 55 13 95 17 91 73 35 5 83 54 29 64 57 59 48 4 52 33 63 39 49 53 82 36 62 24 26 96 92 28 30 11 89 93 40 14 37 34 27 66 69 9 3 77 81 18 41 ]

Cost: 1122.72
Is feasible? yes

WARNING: Large edge lengths in sparse graph
Default Edge Length: 8388607
CCtsp_solve_dat ...
Set initial upperbound to 1122723 (from tour)
infeas_recover ...
phase 1 addbad_variables
LP is now feasible
Recovered a feasible LP
Upperbound from x-heuristic: 1104768.00
Exact lower bound: 1104529.785467
DIFF: 0.000000
LOWER BOUND: 1104529.785467   ACTIVE NODES: 1

Task 0: Branching on node 0
CClp_getweight ...
BBnode 0 split into 1 (1104696.50) 2 (1104642.00) (0.01 seconds)
LOWER BOUND: 1104642.000000   ACTIVE NODES: 2

Task 1: Cutting on node 2
New upperbound from x-heuristic: 1104642.00
TOUR FOUND - upperbound is 1104642.00
Writing Pool: 89 cuts
BBnode 2 (now 3) can be pruned: upperbound 1104642.00 (0.01 seconds)
LOWER BOUND: 1104696.500000   ACTIVE NODES: 1

Task 2: Cutting on node 1
Do not cut, the lp is within 1.0 of the upperbound
Writing Pool: 89 cuts
BBnode 1 (now 4) can be pruned: upperbound 1104642.00 (0.00 seconds)

Task 3: Exit
Total number of nodes in search tree: 3
Optimal tour: 1104642
Total Time to solve TSP: 0.60
cc_return: 0
cc tour: [ 0 41 81 18 9 69 3 77 57 59 48 64 29 54 83 5 39 49 53 82 36 62 63 33 4 52 34 27 66 78 15 16 74 45 65 21 1 20 46 56 19 94 75 72 42 23 97 68 61 25 31 98 89 93 40 37 14 24 26 96 92 11 30 28 12 87 2 60 6 86 70 85 44 67 84 22 8 7 58 55 13 95 17 91 73 35 76 79 47 50 32 38 10 99 80 88 51 43 71 90 ]
Cost: 1104.64
Is feasible? yes
Is optimal? yes

0: 871.91
1: 870.506
2: 880.311
3: 881.703
4: 874.155
5: 874.537
6: 903.863
7: 900.588
8: 895.059
9: 903.207
10: 946.748
11: 1026.41
12: 940.261
13: 946.104
14: 970.106
15: 1067.6
16: 990.15
17: 1048.25
18: 875.35
19: 875.35
20: 870.155
21: 864.592
22: 913.165
23: 938.354
24: 890.103
25: 944.006
26: 959.304
27: 1007.07
28: 930.634
29: 1033.09
30: 950.299
31: 1075.05
32: 949.694
33: 1060.76


Instance: ../../../tspd_instances/singlecenter/singlecenter-alpha_1-99-n100.txt
best cost: 864.592

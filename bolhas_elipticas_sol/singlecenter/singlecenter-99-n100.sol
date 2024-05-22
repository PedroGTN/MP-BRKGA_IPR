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
BBnode 0 split into 1 (1104696.50) 2 (1104642.00) (0.02 seconds)
LOWER BOUND: 1104642.000000   ACTIVE NODES: 2

Task 1: Cutting on node 2
New upperbound from x-heuristic: 1104642.00
TOUR FOUND - upperbound is 1104642.00
Writing Pool: 89 cuts
BBnode 2 (now 3) can be pruned: upperbound 1104642.00 (0.02 seconds)
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

0: 780.944
1: 756.242
2: 763.968
3: 755.007
4: 754.023
5: 764.593
6: 799.521
7: 894.227
8: 769.622
9: 874.369
10: 820.448
11: 989.271
12: 794.299
13: 924.576
14: 821.83
15: 1066.75
16: 838.813
17: 1013.51
18: 838.403
19: 1081.22
20: 850.831
21: 1053.12
22: 838.322
23: 1083.43
24: 850.831
25: 1126.27
26: 770.358
27: 796.764
28: 745.099
29: 791.458
30: 758.373
31: 920.438
32: 753.239
33: 888.922
34: 793.68
35: 1019.52
36: 774.404
37: 982.059
38: 809.713
39: 1083.65
40: 808.739
41: 1027.72
42: 833.416
43: 1183.1
44: 798.111
45: 1054.23
46: 841.098
47: 1165.12
48: 798.111
49: 1152.31


Instance: ../../../tspd_instances/singlecenter/singlecenter-99-n100.txt
best cost: 745.099

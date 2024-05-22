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

0: 779.023
1: 753.241
2: 742.192
3: 722.366
4: 744.644
5: 762.121
6: 780.185
7: 938.141
8: 732.635
9: 892.678
10: 785.309
11: 980.851
12: 806.747
13: 950.312
14: 807.429
15: 974.722
16: 797.859
17: 989.16
18: 796.792
19: 921.014
20: 825.229
21: 1009.21
22: 828.905
23: 958.52
24: 837.3
25: 1063.11
26: 828.905
27: 1107.79
28: 826.82
29: 1115.85
30: 763.004
31: 788.924
32: 735.247
33: 778.202
34: 745.49
35: 948.614
36: 742.535
37: 900.893
38: 769.98
39: 973.975
40: 769.786
41: 952.881
42: 755.315
43: 1024.64
44: 749.349
45: 952.864
46: 760.13
47: 956.361
48: 785.37
49: 992.367
50: 778.549
51: 1049.66
52: 788.97
53: 1084.07
54: 776.973
55: 1065.05
56: 764.615
57: 1178.66


Instance: ../../../tspd_instances/singlecenter/singlecenter-alpha_3-99-n100.txt
best cost: 722.366

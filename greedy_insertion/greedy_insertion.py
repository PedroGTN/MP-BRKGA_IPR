import heapq

def greedy_insertion(graph, initial_tour, N, closest_nodes):

    pass



# Greedy Insert:

# for each v in V’:
# 	heap_insert(v.N[0]) #cada nó inserido atualiza o heap e cada índice do N

# while(V’ != V) do:
# 	next = heap_top() #avança na lista de nós mais próximos
#                    	retirando o ‘next’ e atualiza o heap

# 	#apenas inserir no nó mais próximo que era quem estava no topo do heap

# 	for each a in next.A:
#     	if a in E’:
#         	insert_edge = a
#         	break

# 	if insert_edge == NULL:
#     	insert_edge = random_edge(E’)

# 	heap_insert(next.N[0])

# 	inserir(next, insert_edge, G’)

# return G’

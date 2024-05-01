#define NUM_REPET 1

#include "Tspdsolver.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>


#include "configs.h"

using namespace std;
using namespace chrono;

auto s_time = high_resolution_clock::now(),
     e_time = high_resolution_clock::now();
auto tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
auto tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
auto tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);

int cont_repet = 0;

// Directed multigraph using adjacency list where lists are linked lists
// Verificar se está inserindo no início e não no final das listas de adjacência

template <typename T> Tspd_solver<T>::Tspd_solver(const T &inst):
instance(inst)
{

}

template <typename T> Tspd_solver<T>::~Tspd_solver()
{

}

template <typename T> BRKGA::fitness_t Tspd_solver<T>::decode(BRKGA::Chromosome& chromosome, bool){

    vector<pair<double, unsigned>> permutation(instance.getN());
    vector<int> permutation2(instance.getN());
    chromosome[0] = 0;
    for(unsigned i = 0; i < instance.getN(); ++i){
       permutation[i] = make_pair(chromosome[i], i);
    }

    sort(permutation.begin()+1, permutation.end());


    for(unsigned i = 0; i < instance.getN(); i++)
        permutation2[i] = permutation[i].second;

    double aux = split_lazy(instance, permutation2);

    return aux;
}

template <typename T> double Tspd_solver<T>::split_lazy(T &problem,
                                vector<int> &permutation)
{
    // if (DEBUG >= 1 && cont_repet == 0)
    // {
    //     tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    //     tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    //     tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
    // }

    // s_time for auxiliary graph initialization
    // s_time = high_resolution_clock::now();
    // int triple_count = 0;
    // int edge_count = 0;
    int n = problem.getN();
    permutation.push_back(n);
    
    // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
    // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco
    // usar map ou unordered_map?
    // map<int, pair<int, double>> G_aux;
    // std::map<int, pair<int, double>>::iterator it;
    Digrafo G_aux(n);
    //e_time = high_resolution_clock::now();
    // tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
    // if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    // cout << "Time of initialization for auxiliary graph: "
    //     << (double)tempo_inic.count() / NUM_REPET << endl;
    // // time for auxiliary graph building
    // s_time = high_resolution_clock::now();
    for (int i = 0; i < n; i++)
    {
        int origem = permutation[i];
        int destino = permutation[i + 1];
        // supomos que o truck_speed é sempre igual a 1
        G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
        // adicionar arestas no grafo de listas de adjacência para facilitar
        // encontrar o caminho minimo
        // edge_count++;
    }

    for (int j = 1; j < n; j++){

        int drone_node = permutation[j];
        int launch = permutation[j - 1];
        int rendezvous = permutation[j + 1];
        double custo_pre = problem.getDist(launch, rendezvous) -
                            problem.getDist(launch, drone_node);
        int k_max = n;
        for (int i = j - 1; i >= 0; i--)
        {
            launch = permutation[i];
            rendezvous = permutation[j + 1];
            double drone_delivery_dist = problem.getDist(launch, drone_node) +
                                        problem.getDist(drone_node, rendezvous);
            double drone_delivery_cost =
                problem.getDroneSpeed() * drone_delivery_dist;
            custo_pre += problem.getDist(permutation[i], permutation[i + 1]);
            double custo_pos = 0;
            double truck_delivery_cost = custo_pre + custo_pos;
            double delivery_cost = max(drone_delivery_cost, truck_delivery_cost);
            // triple_count++;
            /* Pode adicionar arcos multiplos */
            G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
            // edge_count++;
            if (drone_delivery_cost <= truck_delivery_cost) // lazy property
                break;

            for (int k = j + 2; k <= k_max; k++)
            {
                rendezvous = permutation[k];
                drone_delivery_dist = problem.getDist(launch, drone_node) +
                                        problem.getDist(drone_node, rendezvous);
                drone_delivery_cost = problem.getDroneSpeed() * drone_delivery_dist;
                custo_pos += problem.getDist(permutation[k - 1], permutation[k]);
                truck_delivery_cost = custo_pre + custo_pos;
                delivery_cost = max(drone_delivery_cost, truck_delivery_cost);
                // triple_count++;
                // atualizar arco (launch, rendezvous) se custo total for menor
                // no caso de um drone mais lento que o caminhao esse arco pode
                // ser pior que o caminhao sozinho e nao precisa ser colocado
                // (futura otimizaca)
                /* Pode adicionar arcos multiplos */
                G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
                // edge_count++;
                if (drone_delivery_cost <= truck_delivery_cost) // lazy property
                {
                    k_max = k - 1;
                    break;
                }
            }
        }
    }
    //e_time = high_resolution_clock::now();
    //tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
    // if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    // cout << "Time of building for auxiliary graph: "
    //     << (double)tempo_gaux.count() / NUM_REPET << endl;
    // s_time for shortest path algorithm
    // s_time = high_resolution_clock::now();
    // distances eh a distancia do deposito 0 ate cada vertice
    vector<double> distances(n + 1, numeric_limits<double>::max());
    distances[0] = 0;
    vector<int> pred(n + 1, -1);
    for (int i = 0; i < n; i++)
    {
        int u = permutation[i];
        arc *vizinhos = G_aux.nodes[u];
        while (vizinhos != NULL)
        {
            int v = vizinhos->dest;
            double arc_cost = vizinhos->cost;
            if (distances[u] + arc_cost < distances[v])
            {
                distances[v] = distances[u] + arc_cost;
                pred[v] = u;
            }
            vizinhos = vizinhos->prox;
        }
    }

    // e_time = high_resolution_clock::now();
    // tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
    // if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    // cout << "Time of shortest path algorithm: "
    //     << (double)tempo_cam_min.count() / NUM_REPET << endl;

    permutation.pop_back();

    // if (DEBUG >= 1 && cont_repet == 0)
    // cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;
    // if (DEBUG >= 1 && cont_repet == 0)
    // cout << "Number of triples considered: " << triple_count << endl;

    return distances[n];
}


template <typename T> double Tspd_solver<T>::split_lazy(T &problem,
                                vector<int> &permutation,
                                vector<int> &predecessor,
                                Digrafo &G_aux)
{
    int n = problem.getN();
    permutation.push_back(n);
    

    for (int i = 0; i < n; i++)
    {
        int origem = permutation[i];
        int destino = permutation[i + 1];
        G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    }

    for (int j = 1; j < n; j++){

        int drone_node = permutation[j];
        int launch = permutation[j - 1];
        int rendezvous = permutation[j + 1];
        double custo_pre = problem.getDist(launch, rendezvous) -
                            problem.getDist(launch, drone_node);
        int k_max = n;
        for (int i = j - 1; i >= 0; i--)
        {
            launch = permutation[i];
            rendezvous = permutation[j + 1];
            double drone_delivery_dist = problem.getDist(launch, drone_node) +
                                        problem.getDist(drone_node, rendezvous);
            double drone_delivery_cost =
                problem.getDroneSpeed() * drone_delivery_dist;
            custo_pre += problem.getDist(permutation[i], permutation[i + 1]);
            double custo_pos = 0;
            double truck_delivery_cost = custo_pre + custo_pos;
            double delivery_cost = max(drone_delivery_cost, truck_delivery_cost);

            G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
            if (drone_delivery_cost <= truck_delivery_cost) 
                break;

            for (int k = j + 2; k <= k_max; k++)
            {
                rendezvous = permutation[k];
                drone_delivery_dist = problem.getDist(launch, drone_node) +
                                        problem.getDist(drone_node, rendezvous);
                drone_delivery_cost = problem.getDroneSpeed() * drone_delivery_dist;
                custo_pos += problem.getDist(permutation[k - 1], permutation[k]);
                truck_delivery_cost = custo_pre + custo_pos;
                delivery_cost = max(drone_delivery_cost, truck_delivery_cost);

                G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
                if (drone_delivery_cost <= truck_delivery_cost) 
                {
                    k_max = k - 1;
                    break;
                }
            }
        }
    }

    vector<double> distances(n + 1, numeric_limits<double>::max());
    distances[0] = 0;
    vector<int> pred(n + 1, -1);
    for (int i = 0; i < n; i++)
    {
        int u = permutation[i];
        arc *vizinhos = G_aux.nodes[u];
        while (vizinhos != NULL)
        {
            int v = vizinhos->dest;
            double arc_cost = vizinhos->cost;
            if (distances[u] + arc_cost < distances[v])
            {
                distances[v] = distances[u] + arc_cost;
                pred[v] = u;
            }
            vizinhos = vizinhos->prox;
        }
    }

    predecessor = pred;
    permutation.pop_back();
    return distances[n];
}
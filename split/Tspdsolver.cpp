/*
 * Tspdsolver.cpp
 *
 *  Created on: 5 de jan. de 2023
 *      Author: hokama
 */

#define NUM_REPET 1

#include "Tspdsolver.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <vector>

#include "Digrafo.h"
#include "Digrafo2.h"
#include "configs.h"

using namespace std;
using namespace chrono;

auto s_time = high_resolution_clock::now(),
     e_time = high_resolution_clock::now();
auto tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
auto tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
auto tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);

int cont_repet;

Tspd_solver::Tspd_solver()
{
  // TODO Auto-generated constructor stub
}

Tspd_solver::~Tspd_solver()
{
  // TODO Auto-generated destructor stub
}

int map2hash(int a, int b) { return (a + b + 1) * (a + b) / 2 + b; }

void Tspd_solver::tsp_sol_print(vector<int> &tsp_solution)
{
  for (unsigned i = 0; i < tsp_solution.size() - 1; i++)
  {
    cout << tsp_solution[i] << ", ";
  }
  cout << tsp_solution[tsp_solution.size() - 1] << endl;
}

double Tspd_solver::tsp_sol_cost(Tspd_problem &problem,
                                 vector<int> &tsp_solution)
{
  if (tsp_solution.size() != problem.getN())
    printf("Solucao tem numero de vertices distinto do grafo de entrada\n");
  double sol_cost = 0;
  for (unsigned int i = 0; i < tsp_solution.size() - 1; i++)
    sol_cost += problem.getDist(tsp_solution[i], tsp_solution[i + 1]);
  sol_cost +=
      problem.getDist(tsp_solution[tsp_solution.size() - 1], tsp_solution[0]);
  return sol_cost;
}

// testar se precisa passar permutation como apontador de vector
// essa heurística é o nearest neighbor
void Tspd_solver::nearest(Tspd_problem &problem, vector<int> &permutation)
{
  int number_of_vertices = problem.getN();
  vector<bool> visited(number_of_vertices, 0);

  // place depot in the solution
  int current = 0;
  permutation.push_back(current);
  visited[current] = true;

  for (int i = 1; i < number_of_vertices; i++)
  {
    int next = -1;
    double next_dist = numeric_limits<double>::max();
    for (int v = 1; v < number_of_vertices; v++)
    {
      if (visited[v] != true && problem.getDist(current, v) < next_dist)
      {
        next = v;
        next_dist = problem.getDist(current, v);
      }
    }
    current = next;
    permutation.push_back(current);
    visited[current] = true;
  }
}

// Esse algoritmo começa com um ciclo e isso pode ter algumas versoes:
// o ciclo pode ser um auto-loop (ai precisa escolher qual cidade - escolhi
// esse, comecando do 0) mas também pode ser 2 cidades quaisquers (e escolher
// essas cidades tb é uma questão)
void Tspd_solver::nearest_insertion(Tspd_problem &problem,
                                    vector<int> &permutation)
{
  int number_of_vertices = problem.getN();
  vector<bool> visited(number_of_vertices, 0);

  // place depot in the solution
  int initial_vert = 0;
  permutation.push_back(initial_vert);
  visited[initial_vert] = true;

  // vou inserir number_of_vertices - 1 vertices
  for (int i = 1; i < number_of_vertices; i++)
  {
    int next = -1;
    double next_dist = numeric_limits<double>::max();
    for (int v = 1; v < number_of_vertices; v++)
    {
      if (visited[v] == true)
        continue;
      for (int u : permutation)
      {
        if (problem.getDist(u, v) < next_dist)
        {
          next = v;
          next_dist = problem.getDist(u, v);
        }
      }
    }
    // next escolhido, decidir onde colocar
    int best_pos = 0; // next vai ser colocado entre  best_pos e best_pos+1
    double best_dist = numeric_limits<double>::max();
    for (unsigned int iu = 0; iu < permutation.size(); iu++)
    {
      unsigned int u = permutation[iu];
      // Se o u for o ultimo da permutacao o prox (uu) eh o primeiro
      unsigned int uu =
          iu == permutation.size() - 1 ? permutation[0] : permutation[iu + 1];
      double delta_dist = problem.getDist(u, next) + problem.getDist(next, uu) -
                          problem.getDist(u, uu);
      if (delta_dist < best_dist)
      {
        best_dist = delta_dist;
        best_pos = iu;
      }
    }

    // escolhido a melhor posição para colocar o next
    permutation.push_back(-1);
    for (int i = permutation.size() - 1; i > best_pos + 1; i--)
    {
      permutation[i] = permutation[i - 1];
    }
    permutation[best_pos + 1] = next;
    visited[next] = true;
  }
}

// Esse algoritmo começa com um ciclo e isso pode ter algumas versoes:
// o ciclo pode ser um auto-loop (ai precisa escolher qual cidade - escolhi
// esse, comecando do 0) mas também pode ser 2 cidades quaisquers (e escolher
// essas cidades tb é uma questão)
void Tspd_solver::cheapest_insertion(Tspd_problem &problem,
                                     vector<int> &permutation)
{
  int number_of_vertices = problem.getN();
  vector<bool> visited(number_of_vertices, 0);

  // place depot in the solution
  int initial_vert = 0;
  permutation.push_back(initial_vert);
  visited[initial_vert] = true;

  // vou inserir number_of_vertices - 1 vertices
  for (int i = 1; i < number_of_vertices; i++)
  {
    int next = -1;
    int best_pos = 0; // next vai ser colocado entre  best_pos e best_pos+1
    double best_dist = numeric_limits<double>::max();

    // verificando cada um dos vertices
    for (int v = 1; v < number_of_vertices; v++)
    {
      if (visited[v] == true)
        continue;
      // verificar onde colocar
      for (unsigned int iu = 0; iu < permutation.size(); iu++)
      {
        unsigned int u = permutation[iu];
        // Se o u for o ultimo da permutacao o prox (uu) eh o primeiro
        unsigned int uu = iu == permutation.size() - 1 ? permutation[0] : permutation[iu + 1];
        double delta_dist = problem.getDist(u, v) + problem.getDist(v, uu) - problem.getDist(u, uu);
        if (delta_dist < best_dist)
        {
          next = v;
          best_dist = delta_dist;
          best_pos = iu;
        }
      }
    }
    // escolhido a melhor posição para colocar o next
    permutation.push_back(-1);
    for (int i = permutation.size() - 1; i > best_pos + 1; i--)
    {
      permutation[i] = permutation[i - 1];
    }
    permutation[best_pos + 1] = next;
    visited[next] = true;
  }
}

double Tspd_solver::split_basico(Tspd_problem &problem,
                                 vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);
  vector<vector<double>> G_aux(n);
  vector<vector<int>> Drone(n);
  for (int u = 0; u < n; u++)
  {
    G_aux[u].resize(n + 1);
    Drone[u].resize(n + 1);
    for (int v = 0; v <= n; v++)
    {
      G_aux[u][v] = numeric_limits<double>::max();
      Drone[u][v] = -1;
      edge_count++;
    }
  }

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux[origem][destino] = problem.getDist(origem, destino);
  }

  for (int i = 0; i < n - 1; i++)
    for (int k = i + 2; k <= n; k++)
    {
      int best_drone_node = -1;
      double base_path_cost = 0;
      for (int l = i; l < k; l++)
        base_path_cost += problem.getDist(permutation[l], permutation[l + 1]);
      double best_delivery_cost = base_path_cost;
      for (int j = i + 1; j < k; j++)
      {
        double drone_delivery_dist =
            problem.getDist(permutation[i], permutation[j]) +
            problem.getDist(permutation[j], permutation[k]);
        double drone_delivery_cost =
            problem.getDroneSpeed() * drone_delivery_dist;

        double removed_edges_cost =
            problem.getDist(permutation[j - 1], permutation[j]) +
            problem.getDist(permutation[j], permutation[j + 1]);
        double added_edge_cost =
            problem.getDist(permutation[j - 1], permutation[j + 1]);
        double truck_delivery_cost =
            base_path_cost - removed_edges_cost + added_edge_cost;

        triple_count++;

        double delivery_cost = max(drone_delivery_cost, truck_delivery_cost);
        if (delivery_cost < best_delivery_cost)
        {
          best_delivery_cost = delivery_cost;
          best_drone_node = permutation[j];
        }
      }
      int origem = permutation[i];
      int destino = permutation[k];
      G_aux[origem][destino] = best_delivery_cost;
      Drone[origem][destino] = best_drone_node;
    }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

  // distances eh a distancia do deposito 0 ate cada vertice
  vector<double> distances(n + 1, numeric_limits<double>::max());
  distances[0] = 0;
  vector<int> pred(n + 1, -1);
  for (int i = 0; i < n; i++)
  {
    int u = permutation[i];
    for (int j = i + 1; j <= n; j++)
    {
      int v = permutation[j];
      if ((G_aux[u][v] < numeric_limits<double>::max() - 1) &&
          distances[u] + G_aux[u][v] < distances[v])
      {
        distances[v] = distances[u] + G_aux[u][v];
        pred[v] = u;
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;
  return distances[n];
}

// Directed graph using adjacency matrix
double Tspd_solver::split_lazy(Tspd_problem &problem,
                               vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);
  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco
  vector<vector<double>> G_aux(n);
  vector<vector<int>> Drone(n);
  for (int u = 0; u < n; u++)
  {
    G_aux[u].resize(n + 1);
    Drone[u].resize(n + 1);
    for (int v = 0; v <= n; v++)
    {
      G_aux[u][v] = numeric_limits<double>::max();
      Drone[u][v] = -1;
      edge_count++;
    }
  }

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux[origem][destino] = problem.getDist(
        origem, destino); // quando usar a hash table colocar drone = -1
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      // atualizar arco (launch, rendezvous) se custo total for menor
      // no caso de um drone mais lento que o caminhao esse arco pode ser
      // pior que o caminhao sozinho e nao precisa ser colocado (futura
      // otimizaca)
      if (delivery_cost < G_aux[launch][rendezvous])
      {
        G_aux[launch][rendezvous] = delivery_cost;
        Drone[launch][rendezvous] = drone_node;
      }
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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)
        if (delivery_cost < G_aux[launch][rendezvous])
        {
          G_aux[launch][rendezvous] = delivery_cost;
          Drone[launch][rendezvous] = drone_node;
        }
        if (drone_delivery_cost <= truck_delivery_cost)
        { // lazy property
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

  // distances eh a distancia do deposito 0 ate cada vertice
  vector<double> distances(n + 1, numeric_limits<double>::max());
  distances[0] = 0;
  vector<int> pred(n + 1, -1);
  for (int i = 0; i < n; i++)
  {
    int u = permutation[i];
    // for (int j = 0; j <= n; j++)
    for (int j = i + 1; j <= n; j++)
    {
      int v = permutation[j];
      if ((G_aux[u][v] < numeric_limits<double>::max() - 1) &&
          distances[u] + G_aux[u][v] < distances[v])
      {
        distances[v] = distances[u] + G_aux[u][v];
        pred[v] = u;
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;
  return distances[n];
}

// Directed graph using adjacency list where lists are dynamic vectors
// and each arc is added to a hash table
double Tspd_solver::split_lazy2(Tspd_problem &problem,
                                vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco

  // usar map ou unordered_map?
  // map<int, pair<int, double>> G_aux;
  // std::map<int, pair<int, double>>::iterator it;

  unordered_map<int, pair<int, double>> G_aux;
  std::unordered_map<int, pair<int, double>>::iterator it;

  vector<vector<int>> graph(n);

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux[map2hash(origem, destino)] = make_pair(
        -1, problem.getDist(
                origem,
                destino)); // quando usar a hash table colocar drone = -1
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    graph[origem].push_back(destino);
    edge_count++;
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      // atualizar arco (launch, rendezvous) se custo total for menor
      // no caso de um drone mais lento que o caminhao esse arco pode ser
      // pior que o caminhao sozinho e nao precisa ser colocado (futura
      // otimizaca)
      int key = map2hash(launch, rendezvous);

      /* Area em contrucao */

      /* Maneira original */

      // // if (G_aux.count(key) == 0)
      // if (G_aux[key].first == 0)
      // {
      //   G_aux[key] = make_pair(drone_node, delivery_cost);
      //   // adiciona o arco no grafo de listas de adjacencia
      //   graph[launch].push_back(rendezvous);
      //   edge_count++;
      // }
      // else if (delivery_cost < G_aux[key].second)
      // {
      //   G_aux[key].first = drone_node;
      //   G_aux[key].second = delivery_cost;
      //   // G_aux[key] = make_pair(drone_node, delivery_cost);
      // }

      /* Maneira Alternativa 1 */

      // double cost = G_aux[key].second;
      // if (cost == 0)
      // {
      //   G_aux[key] = make_pair(drone_node, delivery_cost);
      //   // adiciona o arco no grafo de listas de adjacencia
      //   graph[launch].push_back(rendezvous);
      //   edge_count++;
      // }
      // else if (delivery_cost < cost)
      // {
      //   // G_aux[key].first = drone_node;
      //   // G_aux[key].second = delivery_cost;
      //   G_aux[key] = make_pair(drone_node, delivery_cost);
      // }

      /* Maneira Alternativa 2 */

      it = G_aux.find(key);
      if (it == G_aux.end())
      {
        G_aux[key] = make_pair(drone_node, delivery_cost);
        // adiciona o arco no grafo de listas de adjacencia
        graph[launch].push_back(rendezvous);
        edge_count++;
      }
      // else if (delivery_cost < G_aux.at(key).second)
      else if (delivery_cost < it->second.second)
      {
        // G_aux[key].first = drone_node;
        // G_aux[key].second = delivery_cost;
        it->second.first = drone_node;
        it->second.second = delivery_cost;
        // G_aux[key] = make_pair(drone_node, delivery_cost);
      }

      /* Fim da Area em Construcao */

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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)
        key = map2hash(launch, rendezvous);
        // if (G_aux.count(key) == 0)
        if (G_aux[key].first == 0)
        {
          G_aux[key] = make_pair(drone_node, delivery_cost);
          // adiciona o arco no grafo de listas de adjacencia
          graph[launch].push_back(rendezvous);
          edge_count++;
        }
        else if (delivery_cost < G_aux[key].second)
        {
          G_aux[key].first = drone_node;
          G_aux[key].second = delivery_cost;
          // G_aux[key] = make_pair(drone_node, delivery_cost);
        }
        if (drone_delivery_cost <= truck_delivery_cost)
        { // lazy property
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

  // distances eh a distancia do deposito 0 ate cada vertice
  vector<double> distances(n + 1, numeric_limits<double>::max());
  distances[0] = 0;
  vector<int> pred(n + 1, -1);
  for (int i = 0; i < n; i++)
  {
    int u = permutation[i];
    for (unsigned int j = 0; j < graph[u].size(); j++)
    {
      int v = graph[u][j];
      double arc_cost = G_aux[map2hash(u, v)].second;
      if (distances[u] + arc_cost < distances[v])
      {
        distances[v] = distances[u] + arc_cost;
        pred[v] = u;
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  return distances[n];
}

// Directed graph using adjacency list where lists are linked lists
double Tspd_solver::split_lazy3(Tspd_problem &problem,
                                vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco

  // usar map ou unordered_map?
  // map<int, pair<int, double>> G_aux;
  // std::map<int, pair<int, double>>::iterator it;

  Digrafo G_aux(n);

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    edge_count++;
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      arc *it = G_aux.getArc(launch, rendezvous);
      if (it == NULL)
      {
        G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
        edge_count++;
      }
      // else if (delivery_cost < G_aux.at(key).second)
      else if (delivery_cost < it->cost)
      {
        // G_aux[key].first = drone_node;
        // G_aux[key].second = delivery_cost;
        it->drone_node = drone_node;
        it->cost = delivery_cost;
        // G_aux[key] = make_pair(drone_node, delivery_cost);
      }

      /* Fim da Area em Construcao */

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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)
        it = G_aux.getArc(launch, rendezvous);
        // if (G_aux.count(key) == 0)
        if (it == NULL)
        {
          G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
          edge_count++;
        }
        else if (delivery_cost < it->cost)
        {
          it->drone_node = drone_node;
          it->cost = delivery_cost;
          // G_aux[key] = make_pair(drone_node, delivery_cost);
        }
        if (drone_delivery_cost <= truck_delivery_cost)
        { // lazy property
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

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

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  return distances[n];
}

// Directed graph using adjacency list where lists are dynamic vectors
double Tspd_solver::split_lazy4(Tspd_problem &problem,
                                vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco

  // usar map ou unordered_map?
  // map<int, pair<int, double>> G_aux;
  // std::map<int, pair<int, double>>::iterator it;

  Digrafo2 G_aux(n);

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    edge_count++;
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      arc2 *it = G_aux.getArc(launch, rendezvous);
      if (it == NULL)
      {
        G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
        edge_count++;
      }
      // else if (delivery_cost < G_aux.at(key).second)
      else if (delivery_cost < it->cost)
      {
        // G_aux[key].first = drone_node;
        // G_aux[key].second = delivery_cost;
        it->drone_node = drone_node;
        it->cost = delivery_cost;
        // G_aux[key] = make_pair(drone_node, delivery_cost);
      }

      /* Fim da Area em Construcao */

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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)
        it = G_aux.getArc(launch, rendezvous);
        // if (G_aux.count(key) == 0)
        if (it == NULL)
        {
          G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
          edge_count++;
        }
        else if (delivery_cost < it->cost)
        {
          it->drone_node = drone_node;
          it->cost = delivery_cost;
          // G_aux[key] = make_pair(drone_node, delivery_cost);
        }
        if (drone_delivery_cost <= truck_delivery_cost)
        { // lazy property
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

  // distances eh a distancia do deposito 0 ate cada vertice
  vector<double> distances(n + 1, numeric_limits<double>::max());
  distances[0] = 0;
  vector<int> pred(n + 1, -1);
  for (int i = 0; i < n; i++)
  {
    int u = permutation[i];
    for (unsigned int j = 0; j < G_aux.nodes[u].size(); j++)
    {
      int v = G_aux.nodes[u][j].dest;
      double arc_cost = G_aux.nodes[u][j].cost;
      if (distances[u] + arc_cost < distances[v])
      {
        distances[v] = distances[u] + arc_cost;
        pred[v] = u;
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  return distances[n];
}

// Directed multigraph using adjacency list where lists are linked lists
// Verificar se está inserindo no início e não no final das listas de adjacência
double Tspd_solver::split_lazy5(Tspd_problem &problem,
                                vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco

  // usar map ou unordered_map?
  // map<int, pair<int, double>> G_aux;
  // std::map<int, pair<int, double>>::iterator it;

  Digrafo G_aux(n);

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    edge_count++;
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      /* Pode adicionar arcos multiplos */
      G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
      edge_count++;

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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)

        /* Pode adicionar arcos multiplos */
        G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
        edge_count++;

        if (drone_delivery_cost <= truck_delivery_cost) // lazy property
        {
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

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

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  return distances[n];
}

// Directed multigraph using adjacency list where lists are dynamic vectors
double Tspd_solver::split_lazy6(Tspd_problem &problem,
                                vector<int> &permutation)
{
  if (DEBUG >= 1 && cont_repet == 0)
  {
    tempo_inic = duration_cast<nanoseconds>(s_time - s_time);
    tempo_gaux = duration_cast<nanoseconds>(s_time - s_time);
    tempo_cam_min = duration_cast<nanoseconds>(s_time - s_time);
  }

  // s_time for auxiliary graph initialization
  s_time = high_resolution_clock::now();

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  // G_aux e Drone devem ser fundidos em uma hash table indexada por pares de
  // vértices e cujos conteúdos são o custo e o drone node visitado pelo arco

  // usar map ou unordered_map?
  // map<int, pair<int, double>> G_aux;
  // std::map<int, pair<int, double>>::iterator it;

  Digrafo2 G_aux(n);

  e_time = high_resolution_clock::now();
  tempo_inic += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of initialization for auxiliary graph: "
         << (double)tempo_inic.count() / NUM_REPET << endl;

  // time for auxiliary graph building
  s_time = high_resolution_clock::now();

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    edge_count++;
  }

  for (int j = 1; j < n; j++)
  {
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

      triple_count++;

      /* Pode adicionar arcos multiplos */
      G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
      edge_count++;

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

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)

        /* Pode adicionar arcos multiplos */
        G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
        edge_count++;

        if (drone_delivery_cost <= truck_delivery_cost)
        { // lazy property
          k_max = k - 1;
          break;
        }
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_gaux += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of building for auxiliary graph: "
         << (double)tempo_gaux.count() / NUM_REPET << endl;

  // s_time for shortest path algorithm
  s_time = high_resolution_clock::now();

  // distances eh a distancia do deposito 0 ate cada vertice
  vector<double> distances(n + 1, numeric_limits<double>::max());
  distances[0] = 0;
  vector<int> pred(n + 1, -1);
  for (int i = 0; i < n; i++)
  {
    int u = permutation[i];
    for (unsigned int j = 0; j < G_aux.nodes[u].size(); j++)
    {
      int v = G_aux.nodes[u][j].dest;
      double arc_cost = G_aux.nodes[u][j].cost;
      if (distances[u] + arc_cost < distances[v])
      {
        distances[v] = distances[u] + arc_cost;
        pred[v] = u;
      }
    }
  }

  e_time = high_resolution_clock::now();
  tempo_cam_min += duration_cast<nanoseconds>(e_time - s_time);
  if (DEBUG >= 1 && cont_repet == NUM_REPET - 1)
    cout << "Time of shortest path algorithm: "
         << (double)tempo_cam_min.count() / NUM_REPET << endl;

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  return distances[n];
}

double Tspd_solver::solveTSP(Tspd_problem &problem, int heuristic,
                             vector<int> &tsp_solution)
{
  double sol_cost = -1;

  switch (heuristic)
  {
  case 0:
    if (DEBUG >= 1)
    {
      cout << endl
           << "=== tsp solution nearest ===" << endl;
    }
    nearest(problem, tsp_solution);
    break;

  case 1:
    if (DEBUG >= 1)
    {
      cout << endl
           << "=== tsp solution direct ===" << endl;
    }
    tsp_solution.push_back(0);
    for (unsigned i = 1; i < problem.getN(); i++)
      tsp_solution.push_back(i);
    break;

  case 2:
    if (DEBUG >= 1)
    {
      cout << endl
           << "=== tsp solution reverse ===" << endl;
    }
    tsp_solution.push_back(0);
    for (unsigned i = problem.getN() - 1; i > 0; i--)
      tsp_solution.push_back(i);
    break;

  case 3:
    if (DEBUG >= 1)
    {
      cout << endl
           << "=== tsp solution nearest_insertion ===" << endl;
    }
    nearest_insertion(problem, tsp_solution);
    break;

  case 4:
    if (DEBUG >= 1)
    {
      cout << endl
           << "=== tsp solution cheapest_insertion ===" << endl;
    }
    cheapest_insertion(problem, tsp_solution);
    break;

  default:
    break;
  }

  sol_cost = tsp_sol_cost(problem, tsp_solution);
  if (DEBUG >= 1)
  {
    cout << "tsp sol cost: " << sol_cost << endl;
  }
  if (DEBUG >= 2)
  {
    tsp_sol_print(tsp_solution);
  }

  return sol_cost;
}

double Tspd_solver::solveFSTSP(Tspd_problem &problem, int split,
                               vector<int> &tsp_solution)
{
  auto start_time = high_resolution_clock::now(),
       end_time = high_resolution_clock::now();
  auto tempo = duration_cast<nanoseconds>(end_time - start_time);

  double sol_cost = -1;

  switch (split)
  {
  case 0:
    cout << endl
         << "--- split basic O(n^3) worst and best ---" << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_basico(problem, tsp_solution);
    break;

  case 1:
    cout << endl
         << "--- split lazy O(n^2) best due to graph matrix ---" << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy(problem, tsp_solution);
    break;

  case 2:
    cout << endl
         << "--- split lazy2 O(n) best with edges hash table ---" << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy2(problem, tsp_solution);
    break;

  case 3:
    cout << endl
         << "--- split lazy3 O(n) best with graph linked lists ---" << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy3(problem, tsp_solution);
    break;

  case 4:
    cout << endl
         << "--- split lazy4 O(n) best with graph dynamic vectors ---"
         << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy4(problem, tsp_solution);
    break;

  case 5:
    cout << endl
         << "--- split lazy5 O(n) best with multigraph linked lists ---"
         << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy5(problem, tsp_solution);
    break;

  case 6:
    cout << endl
         << "--- split lazy6 O(n) best with multigraph dynamic vectors "
            "---"
         << endl;
    cout << "Repeating test " << NUM_REPET << " times" << endl;
    for (cont_repet = 0; cont_repet < NUM_REPET; cont_repet++)
      sol_cost = split_lazy6(problem, tsp_solution);
    break;

  default:
    break;
  }

  cout << "FSTSP Sol Cost: " << sol_cost << endl;
  end_time = high_resolution_clock::now();
  tempo = duration_cast<nanoseconds>(end_time - start_time);
  cout << "Average Time (ns): " << (double)tempo.count() / NUM_REPET << endl;

  return sol_cost;

  return 0;
}

double Tspd_solver::findLocality(Tspd_problem &problem,
                                 vector<int> &permutation)
{
  int overall_locality = -1;

  int triple_count = 0;
  int edge_count = 0;
  int n = problem.getN();
  permutation.push_back(n);

  Digrafo G_aux(n);

  vector<int> vertices_locality;

  for (int i = 0; i < n; i++)
  {
    int origem = permutation[i];
    int destino = permutation[i + 1];
    // supomos que o truck_speed é sempre igual a 1
    G_aux.addArc(origem, destino, -1, problem.getDist(origem, destino));
    // adicionar arestas no grafo de listas de adjacência para facilitar
    // encontrar o caminho minimo
    edge_count++;
  }

  vertices_locality.push_back(0);

  for (int j = 1; j < n; j++)
  {
    int vertex_locality = n;

    int drone_node = permutation[j];
    int launch = permutation[j - 1];
    int rendezvous = permutation[j + 1];
    double custo_pre = problem.getDist(launch, rendezvous) -
                       problem.getDist(launch, drone_node);
    int k_max = n;
    for (int i = j - 1; i >= 0; i--)
    {
      int left_locality = j - i;
      int right_locality = 1;

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

      triple_count++;

      /* Pode adicionar arcos multiplos */
      G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
      edge_count++;

      if (drone_delivery_cost <= truck_delivery_cost) // lazy property
      {
        vertex_locality =
            min(vertex_locality, max(left_locality, right_locality));
        break;
      }

      for (int k = j + 2; k <= k_max; k++)
      {
        right_locality = k - j;

        rendezvous = permutation[k];
        drone_delivery_dist = problem.getDist(launch, drone_node) +
                              problem.getDist(drone_node, rendezvous);
        drone_delivery_cost = problem.getDroneSpeed() * drone_delivery_dist;

        custo_pos += problem.getDist(permutation[k - 1], permutation[k]);
        truck_delivery_cost = custo_pre + custo_pos;

        delivery_cost = max(drone_delivery_cost, truck_delivery_cost);

        triple_count++;

        // atualizar arco (launch, rendezvous) se custo total for menor
        // no caso de um drone mais lento que o caminhao esse arco pode
        // ser pior que o caminhao sozinho e nao precisa ser colocado
        // (futura otimizaca)

        /* Pode adicionar arcos multiplos */
        G_aux.addArc(launch, rendezvous, drone_node, delivery_cost);
        edge_count++;

        if (drone_delivery_cost <= truck_delivery_cost) // lazy property
        {
          vertex_locality =
              min(vertex_locality, max(left_locality, right_locality));
          k_max = k - 1;
          break;
        }
      }
    }
    vertices_locality.push_back(vertex_locality);
    overall_locality = max(overall_locality, vertex_locality);
  }

  permutation.pop_back();

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of arcs in the auxiliary graph: " << edge_count << endl;

  if (DEBUG >= 1 && cont_repet == 0)
    cout << "Number of triples considered: " << triple_count << endl;

  cout << "Locality: " << overall_locality << endl;

  cout << "Vertices locality: " << endl;
  int count = 0;
  for (int i = 0; i < n; i++)
  {
    cout << vertices_locality[i] << " ";
    if (vertices_locality[i] > 1)
      count++;
  }
  cout << endl;

  cout << "Circuit: " << endl;
  for (int i = 0; i < n; i++)
    cout << permutation[i] << " ";
  cout << endl;

  cout << "Number of vertices with locality > 1: " << count << endl;

  cout << "Vertices labels: " << endl;
  for (int i = 0; i < n; i++)
    if (vertices_locality[i] > 1)
      cout << permutation[i] << " ";
  cout << endl;

  cout << "Vertices order: " << endl;
  for (int i = 0; i < n; i++)
    if (vertices_locality[i] > 1)
      cout << i << " ";
  cout << endl;

  return 0;
}

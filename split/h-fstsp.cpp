//============================================================================
// Name        : lazy_drone.cpp
// Author      : Hokama and Felice
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include "Tspdproblem.h"
#include "Tspdsolver.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << " instance_name tsp_solution_name "
             << "Y" << endl;
        cout << "Y = 0 --> split_basico" << endl;
        cout << "Y > 0 --> split_lazyY" << endl;
        cout << "Y = -1 --> find_locality" << endl;
        exit(0);
    }

    Tspd_problem problem(argv[1]);
    Tspd_solver solver;
    vector<int> tsp_solution;

    // Lê um circuito hamiltoniano de argv[2] e carrega ele em tsp_solution
    ifstream file_tsp_solution(argv[2]);
    // string linha;
    // getline(file_tsp_solution, linha);
    // cout << linha << endl;
    int node;
    for (unsigned int i = 0; i < problem.getN(); i++)
    {
        file_tsp_solution >> node;
        tsp_solution.push_back(node);
    }

    double sol_cost = solver.tsp_sol_cost(problem, tsp_solution);
    cout << "TSP Sol Cost: " << sol_cost << endl;

    // cout << "tsp_solution.size() = " << tsp_solution.size();
    // for (unsigned int i = 0; i < tsp_solution.size(); i++)
    //     cout << tsp_solution[i] << " ";
    // cout << endl;

    if (atoi(argv[3]) == -1)
    {
        // Calcular a localidade da instancia + circuito do TSP
        solver.findLocality(problem, tsp_solution);
    }
    else
    {
        // Obter solução para o FSTSP a partir de um circuito hamiltoniano
        // usando o algoritmo split escolhido
        solver.solveFSTSP(problem, atoi(argv[3]), tsp_solution);
    }

    return 0;
}

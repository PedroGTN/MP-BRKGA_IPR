//============================================================================
// Name        : lazy_drone.cpp
// Author      : Hokama and Felice
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "Tspdproblem.h"
#include "Tspdsolver.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " instance_name "
             << "X" << endl;
        cout << "X = 0 --> nearest" << endl;
        cout << "X = 1 --> direct" << endl;
        cout << "X = 2 --> reverse" << endl;
        cout << "X = 3 --> nearest_insert" << endl;
        cout << "X = 4 --> cheapest_insert" << endl;
        exit(0);
    }
    Tspd_problem problem(argv[1]);
    Tspd_solver solver;
    vector<int> tsp_solution;

    // Obter solução inicial para o TSP usando o metodo escolhido
    solver.solveTSP(problem, atoi(argv[2]), tsp_solution);

    // Imprime a tsp_solution obtida na saída padrão
    for (unsigned int i = 0; i < tsp_solution.size() - 1; i++)
        cout << tsp_solution[i] << " ";
    cout << tsp_solution[tsp_solution.size() - 1] << endl;

    return 0;
}

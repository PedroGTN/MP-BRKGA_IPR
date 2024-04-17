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
	if (argc < 4)
	{
		cout << "Usage: " << argv[0] << " instance_name "
			 << "X 0" << endl;
		cout << "X = 0 --> nearest" << endl;
		cout << "X = 1 --> direct" << endl;
		cout << "X = 2 --> reverse" << endl;
		exit(0);
	}
	Tspd_problem problem(argv[1]);
	Tspd_solver solver;
	vector<int> tsp_solution;

	// Obter solução inicial para o TSP usando o metodo escolhido
	solver.solveTSP(problem, atoi(argv[2]), tsp_solution);

	// cout << "tsp_solution.size() = " << tsp_solution.size();
	// for (unsigned int i = 0; i < tsp_solution.size(); i++)
	// 	cout << tsp_solution[i] << " ";
	// cout << endl;

	// Obter solução para o FSTSP a partir de um circuito hamiltoniano
	// usando o algoritmo split escolhido
	solver.solveFSTSP(problem, atoi(argv[3]), tsp_solution);

	return 0;
}

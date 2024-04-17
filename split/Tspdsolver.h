/*
 * Tspdsolver.h
 *
 *  Created on: 5 de jan. de 2023
 *      Author: hokama
 */

#ifndef TSPDSOLVER_H_
#define TSPDSOLVER_H_

#include "Tspdproblem.h"

class Tspd_solver
{
public:
	Tspd_solver();
	virtual ~Tspd_solver();
	void tsp_sol_print(vector<int> &tsp_solution);
	double tsp_sol_cost(Tspd_problem &problem, vector<int> &tsp_solution);
	double solveTSP(Tspd_problem &problem, int heuristic, vector<int> &tsp_solution);
	double solveFSTSP(Tspd_problem &problem, int split, vector<int> &tsp_solution);
	void nearest(Tspd_problem &problem, vector<int> &permutation);
	void nearest_insertion(Tspd_problem &problem, vector<int> &permutation);
	void cheapest_insertion(Tspd_problem &problem, vector<int> &permutation); 
	double split_basico(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy2(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy3(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy4(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy5(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy6(Tspd_problem &problem, vector<int> &permutation);
	double findLocality(Tspd_problem &problem, vector<int> &permutation);
};

#endif /* TSPDSOLVER_H_ */

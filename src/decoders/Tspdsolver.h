/*
 * Tspdsolver.h
 *
 *  Created on: 5 de jan. de 2023
 *      Author: hokama
 */

#ifndef TSPDSOLVER_H_
#define TSPDSOLVER_H_

#include "Tspdproblem.h"
#include "tsp/tsp_instance.hpp"
#include "decoders/Digrafo.h"
#include "brkga_mp_ipr/fitness_type.hpp"
#include "brkga_mp_ipr/chromosome.hpp"

class Tspd_solver
{
public:
	Tspd_problem instance;
	Tspd_solver(const Tspd_problem&);
	virtual ~Tspd_solver();
	void tsp_sol_print(vector<int> &tsp_solution);
	double tsp_sol_cost(Tspd_problem &problem, vector<int> &tsp_solution);
	double solveTSP(Tspd_problem &problem, int heuristic, vector<int> &tsp_solution);
	double solveFSTSP(Tspd_problem &problem, int split, vector<int> &tsp_solution);
	void nearest(Tspd_problem &problem, vector<int> &permutation);
	void nearest_insertion(Tspd_problem &problem, vector<int> &permutation);
	void cheapest_insertion(Tspd_problem &problem, vector<int> &permutation); 
	double split_lazy(Tspd_problem &problem, vector<int> &permutation);
	double split_lazy(Tspd_problem &problem, vector<int> &permutation, vector<int> &predecessor, Digrafo &G_aux);
	BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool);
};

#endif /* TSPDSOLVER_H_ */

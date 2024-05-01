/*
 * Tspdsolver.h
 *
 *  Created on: 5 de jan. de 2023
 *      Author: hokama
 */

#ifndef TSPDSOLVER_H_
#define TSPDSOLVER_H_

#include "Digrafo.h"
#include "Tspdproblem.h"
#include "tsp/tsp_instance.hpp"
#include "brkga_mp_ipr/fitness_type.hpp"
#include "brkga_mp_ipr/chromosome.hpp"

template <typename T> class Tspd_solver
{
public:
	T instance;
	Tspd_solver(const T&);
	virtual ~Tspd_solver();
	void tsp_sol_print(vector<int> &tsp_solution);
	double tsp_sol_cost(T &problem, vector<int> &tsp_solution);
	double solveTSP(T &problem, int heuristic, vector<int> &tsp_solution);
	double solveFSTSP(T &problem, int split, vector<int> &tsp_solution);
	void nearest(T &problem, vector<int> &permutation);
	void nearest_insertion(T &problem, vector<int> &permutation);
	void cheapest_insertion(T &problem, vector<int> &permutation); 
	double split_lazy(T &problem, vector<int> &permutation);
	double split_lazy(T &problem, vector<int> &permutation, vector<int> &predecessor, Digrafo &G_aux);
	BRKGA::fitness_t decode(BRKGA::Chromosome& chromosome, bool);
};

#endif /* TSPDSOLVER_H_ */

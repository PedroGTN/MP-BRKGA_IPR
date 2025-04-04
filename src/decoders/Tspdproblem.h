/*
 * tspdproblem.h
 *
 *  Created on: 27 de dez. de 2022
 *      Author: hokama
 */

#ifndef TSPDPROBLEM_H_
#define TSPDPROBLEM_H_

#include <vector>
#include <string>
using namespace std;

class Tspd_problem
{
private:
public:
	double truck_vel; // actually pace, i.e., 1/speed
	double drone_vel; // actually pace, i.e., 1/speed
	unsigned num_nodes;
	vector<double> positions;
	vector<vector<double>> distances;

	Tspd_problem(){}
	Tspd_problem(const std::string& filename);
	virtual ~Tspd_problem();

	// se chamar essa funcao com o vertice n (copia do deposito)
	// ele tem que ser o v
	double getDist(int u, int v);
	unsigned int getN();
	double getTruckSpeed(); // actually pace, i.e., 1/speed
	double getDroneSpeed(); // actually pace, i.e., 1/speed
};

#endif /* TSPDPROBLEM_H_ */

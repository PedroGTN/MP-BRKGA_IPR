/*
 * tspdproblem.h
 *
 *  Created on: 27 de dez. de 2022
 *      Author: hokama
 */

#ifndef TSPDPROBLEM_H_
#define TSPDPROBLEM_H_

#include <vector>
using namespace std;

class Tspd_problem
{
private:
	double truck_speed; // actually pace, i.e., 1/speed
	double drone_speed; // actually pace, i.e., 1/speed
	int n;
	vector<pair<double, double>> positions;
	vector<vector<double>> distances;

public:
	Tspd_problem(char *filename);
	virtual ~Tspd_problem();

	// se chamar essa funcao com o vertice n (copia do deposito)
	// ele tem que ser o v
	double getDist(int u, int v);
	unsigned int getN();
	double getTruckSpeed(); // actually pace, i.e., 1/speed
	double getDroneSpeed(); // actually pace, i.e., 1/speed
};

#endif /* TSPDPROBLEM_H_ */

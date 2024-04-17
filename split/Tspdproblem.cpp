/*
 * tspdproblem.cpp
 *
 *  Created on: 27 de dez. de 2022
 *      Author: hokama
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>

#include "Tspdproblem.h"
#include "configs.h"

Tspd_problem::Tspd_problem(char *filename) : truck_speed(0), drone_speed(0), n(0)
{
	// TODO Auto-generated constructor stub
	std::ifstream entrada(filename);
	std::string lixo;
	getline(entrada, lixo);
	DEBUG >= 2 ? cout << "lixo: " << lixo << endl : cout << "";
	entrada >> truck_speed >> lixo;
	getline(entrada, lixo);
	DEBUG >= 2 ? cout << "lixo: " << lixo << endl : cout << "";
	entrada >> drone_speed >> lixo;
	getline(entrada, lixo);
	DEBUG >= 2 ? cout << "lixo: " << lixo << endl : cout << "";
	entrada >> n >> lixo;
	getline(entrada, lixo);
	DEBUG >= 2 ? cout << "lixo: " << lixo << endl : cout << "";
	double x, y;
	entrada >> x >> y >> lixo >> lixo; // coordenadas do deposito
	positions.push_back(make_pair(x, y));
	getline(entrada, lixo);
	DEBUG >= 2 ? cout << "lixo: " << lixo << endl : cout << "";
	for (int i = 1; i < n; i++)
	{
		entrada >> x >> y >> lixo; // coordenadas do deposito
		positions.push_back(make_pair(x, y));
	}

	distances.resize(n);
	for (int u = 0; u < n; u++)
	{
		// o n+1-esimo eh uma copia do deposito
		distances[u].resize(n + 1);
	}

	for (int u = 0; u < n; u++)
	{
		for (int v = 0; v < n; v++)
		{
			distances[u][v] = sqrt(
				pow(positions[u].first - positions[v].first, 2) +
				pow(positions[u].second - positions[v].second, 2));
		}
		distances[u][n] = sqrt(
			pow(positions[u].first - positions[0].first, 2) +
			pow(positions[u].second - positions[0].second, 2));
	}

	if (DEBUG >= 1)
	{
		cout << "Instance Data" << endl;
		cout << "Truck Speed: " << truck_speed << endl;
		cout << "Drone Speed: " << drone_speed << endl;
		cout << "Number of Nodes: " << n << endl;
	}
	if (DEBUG >= 2)
	{
		cout << "Coordenadas dos vertices: " << endl;
		for (int i = 0; i < n; i++)
		{
			cout << i << ": " << setprecision(17) << positions[i].first << ", " << positions[i].second << endl;
		}
	}
	if (DEBUG >= 3)
	{
		cout << "Distancias:" << endl;
		for (int u = 0; u < n; u++)
		{
			cout << u << ": ";
			for (int v = 0; v < n; v++)
			{
				cout << getDist(u, v) << ", ";
			}
			cout << endl;
		}
	}
}

Tspd_problem::~Tspd_problem()
{
	// TODO Auto-generated destructor stub
}

double Tspd_problem::getDist(int u, int v)
{
	// pré calcular as distancias entre cada par de vértices
	// e usar a matriz distances[][] caso trabalhando com algoritmos populacionais
	// return distances[u][v];

	if (v == n)
		v = 0;
	return sqrt(
		pow(positions[u].first - positions[v].first, 2) +
		pow(positions[u].second - positions[v].second, 2));
}

unsigned int Tspd_problem::getN() { return n; }

double Tspd_problem::getTruckSpeed() { return truck_speed; } // actually pace, i.e., 1/speed
double Tspd_problem::getDroneSpeed() { return drone_speed; } // actually pace, i.e., 1/speed

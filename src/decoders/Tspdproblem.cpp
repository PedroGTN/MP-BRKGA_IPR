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

Tspd_problem::Tspd_problem(const std::string& filename) : 	truck_vel(0), 
															drone_vel(0), 
															num_nodes(0),
															positions(),
															distances()
{
	ifstream file(filename, ios::in);
    if(!file){
        cout<<filename;
        throw runtime_error("Cannot open instance file");
    }
    
    file.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        file >> truck_vel;
        file >> drone_vel;
        file >> num_nodes;
        //cout<<num_nodes<<endl;
        positions.resize(num_nodes*2);
        distances.resize(pow(num_nodes,2));
        for(unsigned i = 0; i < positions.size(); ++i){
            file >> positions[i];
            //cout << positions[i] << endl;
        }
        //distance_calc();
    }
    catch(std::ifstream::failure& e) {
        throw fstream::failure("Error reading the instance file");
    }

	// distances.resize(num_nodes);
	// for (unsigned u = 0; u < num_nodes; u++)
	// {
	// 	// o n+1-esimo eh uma copia do deposito
	// 	distances[u].resize(num_nodes + 1);
	// }

	// for (unsigned u = 0; u < num_nodes; u++)
	// {
	// 	for (unsigned v = 0; v <= u; v++)
	// 	{
	// 		double aux = sqrt(
	// 		pow((positions[u*2] - positions[v*2]), 2) +
    // 		pow((positions[u*2 + 1] - positions[v*2 +1]), 2));

	// 		distances[u][v] = aux;
	// 		distances[v][u] = aux;
	// 	}
	// 	distances[u][num_nodes] = sqrt(
	// 		pow(positions[u*2] - positions[0], 2) +
	// 		pow(positions[u*2 + 1] - positions[1], 2));
	// }

	// if (DEBUG >= 1)
	// {
	// 	cout << "Instance Data" << endl;
	// 	cout << "Truck Speed: " << truck_vel << endl;
	// 	cout << "Drone Speed: " << drone_vel << endl;
	// 	cout << "Number of Nodes: " << num_nodes << endl;
	// }
	// if (DEBUG >= 2)
	// {
	// 	cout << "Coordenadas dos vertices: " << endl;
	// 	for (unsigned i = 0; i < num_nodes; i++)
	// 	{
	// 		cout << i << ": " << setprecision(17) << positions[i*2] << ", " << positions[i*2 + 1] << endl;
	// 	}
	// }
	// if (DEBUG >= 3)
	// {
	// 	cout << "Distancias:" << endl;
	// 	for (unsigned u = 0; u < num_nodes; u++)
	// 	{
	// 		cout << u << ": ";
	// 		for (unsigned v = 0; v < num_nodes; v++)
	// 		{
	// 			cout << getDist(u, v) << ", ";
	// 		}
	// 		cout << endl;
	// 	}
	// }
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

	if (v == int(num_nodes))
		v = 0;
	return sqrt(
		pow(positions[u*2] - positions[v*2], 2) +
		pow(positions[u*2 + 1] - positions[v*2 + 1], 2));
}

unsigned int Tspd_problem::getN() { return num_nodes; }

double Tspd_problem::getTruckSpeed() { return truck_vel; } // actually pace, i.e., 1/speed
double Tspd_problem::getDroneSpeed() { return drone_vel; } // actually pace, i.e., 1/speed

#ifndef AVALIADOR_H
#define AVALIADOR_H

using namespace std;

#include "../decoders/Tspdproblem.h"
#include "../decoders/Tspdsolver.h"
#include "../decoders/Digrafo.h"
#include <iostream>
#include <vector>

typedef struct operation {
    int origin;
    int drone_node;
    int rendezvous;
} tOperation;

class avaliador
	{
	public:
		avaliador(vector<int> tour, double tour_cost, string instance_file) : og_tour(tour), og_tour_value(tour_cost){
            instance = Tspd_problem(instance_file);
            drone_operations.resize(instance.getN());
            avaliar();
        }   
		~avaliador(){

        }
		double avaliar();
        bool verificar();
        vector<int> og_tour;
        vector<tOperation> drone_operations;
        double tour_value;
        double og_tour_value;
        Tspd_problem instance;
	};

#endif //AVALIADOR_H
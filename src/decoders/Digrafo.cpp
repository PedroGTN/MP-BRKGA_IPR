/*
 * tspdproblem.cpp
 *
 *  Created on: 27 de dez. de 2022
 *      Author: hokama and felice
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>

#include "Digrafo.h"
#include "configs.h"

Digrafo::Digrafo(int n) : nodes()
{
	nodes.resize(n);
	for (int i = 0; i < n; i++)
	{
		nodes[i] = NULL;
	}
}

Digrafo::~Digrafo()
{
	for(uint32_t i=0; i<nodes.size(); i++){
		arc *del = nodes[i], *aux = nodes[i];

		// for(aux = del->prox; aux!= nullptr; del = aux, aux = del->prox)	delete del;

		// if(del != nullptr) delete del;

		while(aux != nullptr){
			del = aux;
			aux = del->prox;
			delete del;
		}
	}
}

arc *Digrafo::getArc(int ori, int dest)
{
	arc *c_arc = nodes[ori];
	while (c_arc != NULL)
	{
		if (c_arc->dest == dest)
			return c_arc;
		c_arc = c_arc->prox;
	}
	return NULL;
}

void Digrafo::addArc(int ori, int dest, int drone_node, double cost)
{
	arc *n_arc = new arc;
	n_arc->dest = dest;
	n_arc->drone_node = drone_node;
	n_arc->cost = cost;
	n_arc->prox = nodes[ori];
	nodes[ori] = n_arc;
}

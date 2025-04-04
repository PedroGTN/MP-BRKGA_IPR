/*
 * Digrafo.h
 *
 *  Created on: 5 de jan. de 2023
 *      Author: Hokama and Felice
 */

#ifndef DIGRAFO_H_
#define DIGRAFO_H_

#include "Tspdproblem.h"
#include <vector>

typedef struct arc
{
    int dest;
    int drone_node;
    double cost;
    struct arc *prox;
} arc;

class Digrafo
{
public:
    vector<arc *> nodes;
    Digrafo(int n);
    virtual ~Digrafo(); // lembrar de implementar o destrutor pra desalocar a memória usada
    arc *getArc(int ori, int dest);
    void addArc(int ori, int dest, int drone_node, double cost);
};

#endif /* DIGRAFO_H_ */

/*
 * Digrafo.h
 *
 *  Created on: 5 de jan. de 2023
 *      Author: Hokama and Felice
 */

#ifndef DIGRAFO2_H_
#define DIGRAFO2_H_

#include "Tspdproblem.h"
#include <vector>

typedef struct arc2
{
    int dest;
    int drone_node;
    double cost;
} arc2;

class Digrafo2
{
public:
    vector<vector<arc2>> nodes;
    Digrafo2(int n);
    virtual ~Digrafo2(); // lembrar de implementar o destrutor pra desalocar a memória usada
    arc2 *getArc(int ori, int dest);
    // std::vector<arc2>::iterator getArc(int ori, int dest);
    // int getArc(int ori, int dest);
    void addArc(int ori, int dest, int drone_node, double cost);
};

#endif /* DIGRAFO2_H_ */

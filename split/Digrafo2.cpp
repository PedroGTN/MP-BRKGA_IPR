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

#include "Digrafo2.h"
#include "configs.h"

Digrafo2::Digrafo2(int n)
{
    nodes.resize(n);
    for (int i = 0; i < n; i++)
    {
        nodes[i].reserve(10);
    }
}

Digrafo2::~Digrafo2()
{
    // TODO Auto-generated destructor stub
}

// int Digrafo2::getArc(int ori, int dest)
// {
//     int i;
//     for (i = (int)nodes[ori].size() - 1; i >= 0 && nodes[ori][i].dest != dest; i--)
//         ;
//     return i;
// }

arc2 *Digrafo2::getArc(int ori, int dest)
{
    int i;
    for (i = (int)nodes[ori].size() - 1; i >= 0 && nodes[ori][i].dest != dest; i--)
        ;
    if (i != -1)
        return &(nodes[ori][i]);
    return NULL;
}

// std::vector<arc2>::iterator Digrafo2::getArc(int ori, int dest)
// {
//     std::vector<arc2>::iterator it;
//     for (it = nodes[ori].begin(); it < nodes[ori].end() && it->dest != dest; it++)
//         ;
//     return it;
// }

void Digrafo2::addArc(int ori, int dest, int drone_node, double cost)
{
    // nodes[ori].push_back({dest, drone_node, cost});

    arc2 n_arc;
    n_arc.dest = dest;
    n_arc.drone_node = drone_node;
    n_arc.cost = cost;
    nodes[ori].push_back(n_arc);
}

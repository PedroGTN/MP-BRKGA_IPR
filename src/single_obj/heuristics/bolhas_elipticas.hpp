#ifndef BOLHAS_ELIPTICAS_HPP_
#define BOLHAS_ELIPTICAS_HPP_

#include <vector>
#include <algorithm>
#include <cstdint>
#include "decoders/Tspdproblem.h"

/**
 * \brief Modifica um tour de TSP para fazê-lo melhor para o h-FSTSP
 *
 * \param instance a TSP instance.
 *
 * \returns O novo tour
 */

void bolhas_elipticas_diretas(vector<uint64_t> &tour, Tspd_problem &instance, double relaxacao){
    uint64_t i = 1;
    uint64_t n = tour.size();

    while(i < n-1){
        for(uint64_t j = i+1; j < n; j++){

            double dist1 = instance.getDist(tour[i-1], tour[j]);

            double dist2 = instance.getDist(tour[j], tour[i]); 

            double dist_focal = instance.getDist(tour[i-1], tour[i]) * relaxacao;
            
            double alpha = instance.drone_vel;

            if(dist1 + dist2 <= dist_focal/alpha){    

                uint64_t aux = tour[j];

                for(uint64_t k=j; k>i; k--)
                    tour[k] = tour[k-1];

                tour[i] = aux;

                i++;
                break;
            }
        } 
        
        i++;
    }
}

void bolhas_elipticas_reversas(vector<uint64_t> &tour, Tspd_problem &instance, double relaxacao){

    uint64_t i = 1;
    uint64_t n = tour.size();

    while(i < n-1){
        for(uint64_t j = n-1; j >= i+1; j--){

            double dist1 = instance.getDist(tour[i-1], tour[j]);

            double dist2 = instance.getDist(tour[i], tour[j]); 

            double dist_focal = instance.getDist(tour[i-1], tour[i]) * relaxacao;
            
            double alpha = instance.drone_vel;
            
            if(dist1 + dist2 <= dist_focal/alpha){

                uint64_t aux = tour[j];

                for(uint64_t k=j; k>i; k--)
                    tour[k] = tour[k-1];

                tour[i] = aux;

                i++;
                break;
            }
        }

        i++;
    }
}

#endif // BOLHAS_ELIPTICAS_HPP_

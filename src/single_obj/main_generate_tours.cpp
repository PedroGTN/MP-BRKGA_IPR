/******************************************************************************
 * main_minimal.cpp: minimal code for calling BRKGA algorithms to solve
 *                   instances of the Traveling Salesman Problem.
 *
 * (c) Copyright 2015-2023, Carlos Eduardo de Andrade.
 * All Rights Reserved.
 *
 * Created on : Mar 05, 2019 by ceandrade
 * Last update: Sep 20, 2023 by ceandrade
 *
 * This code is released under BRKGA-MP-IPR License:
 * https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#define SCALER 1000

#include "tsp/tsp_instance.hpp"
#include "decoders/tsp_decoder.hpp"
#include "decoders/Tspdproblem.h"
#include "decoders/Tspdsolver.h"
#include "decoders/Digrafo.h"
#include "brkga_mp_ipr.hpp"
#include "heuristics/greedy_tour.hpp"
#include "source/discorde-cpp-api/discorde_cpp.h"
#include "heuristics/bolhas_elipticas.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <queue>

using namespace std;

void printvec(vector<uint64_t> vec){

    for(uint64_t i : vec)
        cout<<i<<" ";

    cout<<endl;
}

void print_tour(vector<pair<arc*,int>> final_tour, int n){

    for(int64_t j=(int64_t)final_tour.size()-1; j>=0; j--){
        cout<<final_tour[j].second<<","<<final_tour[j].first->drone_node<<","<<(final_tour[j].first->dest==n?0:final_tour[j].first->dest)<<"|";
    }

    cout<<endl;
}

uint64_t mergesort_countinv(vector<uint64_t> arr, vector<uint64_t> arr2)
{
    uint64_t size = arr.size();

	vector<uint64_t> aux_arr(size), *pt1, *pt2, auxr(size);

	for(uint64_t i=0; i<size; i++)
		auxr[arr2[i]] = i;

	pt1 = &arr, pt2 = &aux_arr;

	uint64_t inv_num = 0;

	for(uint64_t i=2; i<size*2; i<<=1)
	{
		for(uint64_t j=0; j<size; j+=i)
		{
			uint64_t k=j, l=j+(i/2), aux=j+(i/2), aux2=j;
			uint64_t aux3 = (i/2)+j;

			while(l<j+i && l<size && k<aux && k<size)
			{
				if(auxr[pt1[0][k]]<auxr[pt1[0][l]])
					pt2[0][aux2++] = pt1[0][k++];
				else
					pt2[0][aux2++] = pt1[0][l++], inv_num += aux3 - k;
			}

			while(l<j+i && l<size)
				pt2[0][aux2++] = pt1[0][l++];

			while(k<aux && k<size)
				pt2[0][aux2++] = pt1[0][k++];
		}
		swap(pt1, pt2);
	}

	return inv_num;
}

//-------------------------------[ Main ]------------------------------------//


int main(int argc, char* argv[]) {
    if(argc < 2) {
        //arrumar mensagem
        cerr
        << "Usage: " << argv[0]
        <<" <tsp-instance-file>"
        << endl;
        return 1;
    }

    try {

        const string instance_file = argv[1];

        //cout << "Reading data..." << endl;
        Tspd_problem instance = Tspd_problem(instance_file);
        Tspd_solver decoder(instance);	 

        TSP_Instance inst(instance_file);
        TSP_Decoder tsp_decoder(inst);

        auto start_matrix = chrono::high_resolution_clock::now();		

        srand(0);

        // Create a random costs matrix
        int32_t** cost_matrix = new int32_t*[instance.num_nodes];
        for (uint64_t i = 0; i < instance.num_nodes; ++i) {
            cost_matrix[i] = new int32_t[instance.num_nodes];
        }
        for (uint64_t i = 0; i < instance.num_nodes; ++i) {
            cost_matrix[i][i] = 0;
            for (uint64_t j = i + 1; j < instance.num_nodes; ++j) {
                cost_matrix[i][j] = round(instance.getDist(i, j)*SCALER);
                cost_matrix[j][i] = cost_matrix[i][j];
            }
        }

        cout.setstate(ios_base::failbit);

        auto end_matrix = chrono::high_resolution_clock::now();	

        // =========================================================================
        // Solve the problem
        // =========================================================================

        // Allocate resources to store the solution from Lin-Kernighan heuristic
        auto start_lk = chrono::high_resolution_clock::now();		

        int* lk_tour = new int[instance.num_nodes];
        double lk_cost;
        int lk_return;

        // Solve the problem using Lin-Kernighan heuristic
        lk_return = discorde::linkernighan_full(instance.num_nodes, cost_matrix, lk_tour, &lk_cost);
        auto end_lk = chrono::high_resolution_clock::now();		

        // Allocate resources to store the solution from Concorde solver
        auto start_cc = chrono::high_resolution_clock::now();		

        int* cc_tour = new int[instance.num_nodes];
        double cc_cost;
        int cc_return;
        int cc_status;

        // Solve the problem using Concorde solver (using the tour obtained previously
        // with Lin-Kernighan heuristic as a starting solution, if any was found)
        int* cc_start = (lk_return == DISCORDE_RETURN_OK ? lk_tour : NULL);
        cc_return = discorde::concorde_full(instance.num_nodes, cost_matrix, cc_tour, &cc_cost, &cc_status, cc_start);
        auto end_cc = chrono::high_resolution_clock::now();		


        auto start_bol_el = chrono::high_resolution_clock::now();	
        vector<vector<uint64_t>> tours; 
        vector<string> tour_names;
        vector<uint64_t> aux(instance.num_nodes);
        vector<uint64_t> aux2;

        tour_names.push_back("TSP_OPT");
        tour_names.push_back("LKH_SOL");


        for(uint64_t i=0; i < instance.num_nodes; i++)
            aux[i] = cc_tour[i];
        
        tours.push_back(aux);

        // printvec(tours[0]);

        for(uint64_t i=0; i < instance.num_nodes; i++)
            aux[i] = lk_tour[i];

        tours.push_back(aux);
        // printvec(tours[1]);

        aux.assign(1, 0); aux.insert(aux.end(), tours[1].rbegin(), tours[1].rend()-1);

        // printvec(aux);

        if(mergesort_countinv(tours[0], tours[1]) > mergesort_countinv(tours[0], aux))
            tours[1] = aux;
        
        // cout<<endl<<"0:  ";
        // printvec(tours[0]);
        // cout<<"1:  ";
        // printvec(tours[1]);
        // cout<<endl;

        double alpha = 1/instance.drone_vel;

        for(int i=0; i<2; i++){ //otimo e lkh
            for(double d=0.5; d<2.1; d+=0.25){ //relaxamento
                if(alpha*d > 1){
                    //ordem original
                    aux = tours[i];
                    bolhas_elipticas_diretas(aux, instance, d);
                    tours.push_back(aux);
                    tour_names.push_back(tour_names[i] + "_BED_ORIG_" + to_string(d));
                    tour_names.back().erase(tour_names.back().end()-4, tour_names.back().end());

                    aux = tours[i];
                    bolhas_elipticas_reversas(aux, instance, d);
                    tours.push_back(aux);
                    tour_names.push_back(tour_names[i] + "_BER_ORIG_" + to_string(d));
                    tour_names.back().erase(tour_names.back().end()-4, tour_names.back().end());

                    //ordem inversa
                    aux.assign(1, 0); aux.insert(aux.end(), tours[i].rbegin(), tours[i].rend()-1);
                    bolhas_elipticas_diretas(aux, instance, d);
                    aux2.assign(1, 0); aux2.insert(aux2.end(), aux.rbegin(), aux.rend()-1);
                    tours.push_back(aux2);
                    tour_names.push_back(tour_names[i] + "_BED_INV_" + to_string(d));
                    tour_names.back().erase(tour_names.back().end()-4, tour_names.back().end());

                    aux.assign(1, 0); aux.insert(aux.end(), tours[i].rbegin(), tours[i].rend()-1);
                    bolhas_elipticas_reversas(aux, instance, d);
                    aux2.assign(1, 0); aux2.insert(aux2.end(), aux.rbegin(), aux.rend()-1);
                    tours.push_back(aux2);
                    tour_names.push_back(tour_names[i] + "_BER_INV_" + to_string(d));
                    tour_names.back().erase(tour_names.back().end()-4, tour_names.back().end());
                }
            }
        }

        auto end_bol_el = chrono::high_resolution_clock::now();	


        // for(uint64_t i=2; i<tours.size(); i++) {
        //     cout<<i<<":  ";
        //     printvec(tours[i]);
        // }

        // cout<<endl;

        // for(auto s : tour_names){
        //     cout<<s<<endl;
        // }

        // for(int i=0; i<tour_names.size(); i++)
        //     cout<<tour_names[i]<<endl;

        auto start_find_best = chrono::high_resolution_clock::now();	

        vector<double> sol_costs;
        uint64_t best = 0;
        vector<vector<pair<arc*,int>>> fstsp_tours;
        vector<Digrafo> graphs(tours.size(), Digrafo(instance.getN()));
        for(uint64_t i=0; i<tours.size(); i++){

            // cout<<i<<": ";
            vector<int> permutation, predecessor;
            // Digrafo graph(instance.getN()); 
            // cout<<"TSP tour:\n";
            for(auto j : tours[i]) {
                // cout<<j<<" ";
                permutation.push_back(j);
            }
            // cout<<endl<<endl;
            double cost = decoder.split_lazy(instance, permutation, predecessor, graphs[i]);
            
            vector<pair<arc*,int>> final_tour;
            int ant = instance.getN();
            for(int j=predecessor[ant]; j>=0; j = predecessor[j]){
                arc *a = graphs[i].nodes[j];
                final_tour.push_back(make_pair(a, j));
                int len = final_tour.size()-1;
                while(a->prox!=nullptr){

                    if((a->dest == ant && a->cost < final_tour[len].first->cost) || (a->dest == ant && final_tour[len].first->dest != ant)){
                        final_tour[len] = make_pair(a, j);
                    }
                    
                    a = a->prox;
                }
                ant = j;
            }

            fstsp_tours.push_back(final_tour);

            // cout<<cost<<endl;
            sol_costs.push_back(cost);

            if(sol_costs[best]>sol_costs[i])
                best = i;
            
        }

        auto end_find_best = chrono::high_resolution_clock::now();	

        cout.clear();

        cout<<endl;
        std::chrono::duration<double> elapsed_seconds = end_lk - start_matrix;
        cout<<"LKH ELAPSED TIME: "<<elapsed_seconds.count()<<endl;
        elapsed_seconds = end_cc - start_cc;
        cout<<"TSP OPT SOL ELAPSED TIME: "<<elapsed_seconds.count()<<endl;
        elapsed_seconds = end_bol_el - start_bol_el;
        cout<<"BOL_EL ELAPSED TIME: "<<elapsed_seconds.count()<<endl;


        std::mt19937 rng(9);
        cout<<"Instance: "<<instance_file<<endl;
        for(uint64_t i=0; i<2; i++){

            vector<double> keys(instance.num_nodes);
            for(auto& key : keys)
                key = generate_canonical<double, numeric_limits<double>::digits>(rng);
            

            sort(keys.begin(), keys.end());

            // Then, we visit each node in the tour and assign to it a key.
            BRKGA::Chromosome chromosome(instance.num_nodes);

            for(size_t j = 0; j < keys.size(); j++)
                chromosome[tours[i][j]] = keys[j];

            cout<<tour_names[i]<<" : TSP_COST : "<<tsp_decoder.decode(chromosome, false)<<endl;
        }
        cout<<"best fstsp sol name: "<<tour_names[best]<<endl;
        cout<<"best fstsp sol cost: "<<sol_costs[best]<<endl;
        for(uint64_t i=0; i<tours.size(); i++){
            cout<<tour_names[i]<<" : TSP_TOUR : ";
            printvec(tours[i]);
            cout<<tour_names[i]<<" : FSTSP_TOUR : ";
            print_tour(fstsp_tours[i], instance.getN());
            cout<<tour_names[i]<<" : FSTSP_COST : "<<sol_costs[i]<<endl;
        }


        for (uint64_t i = 0; i < instance.num_nodes; ++i) {
            delete[] cost_matrix[i];
        }
        delete[] cost_matrix;
        delete[] lk_tour;
        delete[] cc_tour;
    }
    catch(exception& e) {
        cerr
        << "\n" << string(40, '*') << "\n"
        << "Exception Occurred: " << e.what()
        << "\n" << string(40, '*')
        << endl;
        return 1;
    }
    return 0;
}

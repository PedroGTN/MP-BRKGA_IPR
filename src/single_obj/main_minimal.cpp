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
#include "avaliador/avaliador.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

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
    if(argc < 7) {
        //arrumar mensagem
        cerr
        << "Usage: " << argv[0]
        << " <seed> <config-file> <maximum-running-time>"
        << " <threads> <mode> <tsp-instance-file>"
        << endl;
        return 1;
    }

    try {
        ////////////////////////////////////////
        // Read command-line arguments and the instance
        ////////////////////////////////////////
        auto start = chrono::high_resolution_clock::now();

        const unsigned seed = stoi(argv[1]);
        const string config_file = argv[2];
        //argv[3] tempo em segundos de execução
        const unsigned num_threads = stoi(argv[4]);
        const unsigned method = stoi(argv[5]);
        const string instance_file = argv[6];
        const string method_name[] = {"no_initPop", "rand_initPop", "def_initPop"};

        //cout << "Reading data..." << endl;
        auto instance = Tspd_problem(instance_file);

        ////////////////////////////////////////
        // Read algorithm parameters
        ////////////////////////////////////////

        //cout << "Reading parameters..." << endl;

        auto [brkga_params, control_params] =
            BRKGA::readConfiguration(config_file);

        // Overwrite the maximum time from the config file.
        control_params.maximum_running_time = chrono::seconds {stoi(argv[3])};

        ////////////////////////////////////////
        // Build the BRKGA data structures
        ////////////////////////////////////////

        //cout << "Building BRKGA data and initializing..." << endl;

        Tspd_solver decoder(instance);

        BRKGA::BRKGA_MP_IPR<Tspd_solver> algorithm(
            decoder, BRKGA::Sense::MINIMIZE, seed,
            instance.num_nodes, brkga_params, num_threads
        );

        ////////////////////////////////////////
        // Find good solutions / evolve
        ////////////////////////////////////////

        //cout << "Running for " << control_params.maximum_running_time << "..."
          //   << endl;
			 
		
		double best = numeric_limits<double>::max();
		{ // local scope to deallocate memory.
        // To inject the initial tour, we need to create chromosome representing
        // that solution. First, we create a set of keys to be used in the
        // chromosome.

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


        // =========================================================================
        // Solve the problem
        // =========================================================================

        fpos_t pos;
        fgetpos(stdout, &pos);  // save the position in the file stream
        int fd = dup(fileno(stdout));  // use the dup() function to create a copy of stdou
        freopen("/dev/null", "w", stdout);
        
        int fde = ::open("/dev/null", O_WRONLY);
        ::dup2(fde, 2);
        ::close(fde);


        // Allocate resources to store the solution from Lin-Kernighan heuristic
        int* lk_tour = new int[instance.num_nodes];
        double lk_cost;
        int lk_return;

        // Solve the problem using Lin-Kernighan heuristic
        lk_return = discorde::linkernighan_full(instance.num_nodes, cost_matrix, lk_tour, &lk_cost);

        // cout << "lk tour: [ ";
        // for (uint64_t i = 0; i < instance.num_nodes; ++i) {
        //     cout << lk_tour[i] << " ";
        // }
        // cout << "]" << endl << endl;
        // cout << "Cost: " << lk_cost/SCALER << endl;
        // cout << "Is feasible? " << ((lk_return == DISCORDE_RETURN_OK) ? "yes" : "no") << endl << endl;

        // Allocate resources to store the solution from Concorde solver
        int* cc_tour = new int[instance.num_nodes];
        double cc_cost;
        int cc_return;
        int cc_status;

        // Solve the problem using Concorde solver (using the tour obtained previously
        // with Lin-Kernighan heuristic as a starting solution, if any was found)
        int* cc_start = (lk_return == DISCORDE_RETURN_OK ? lk_tour : NULL);
        cc_return = discorde::concorde_full(instance.num_nodes, cost_matrix, cc_tour, &cc_cost, &cc_status, cc_start);

        fflush(stdout);   
        dup2(fd, fileno(stdout));  // restore the stdout
        close(fd);
        clearerr(stdout);  

        fsetpos(stdout, &pos); // move to the correct position

        // cout<<"cc_return: "<<cc_return;
        // cout << "\ncc tour: [ ";
        // for (uint64_t i = 0; i < instance.num_nodes; ++i) {
        //     cout << cc_tour[i] << " ";
        // }
        // cout << "]" << endl;
        // cout << "Cost: " << cc_cost/SCALER << endl;
        // cout << "Is feasible? " << ((cc_return == DISCORDE_RETURN_OK) ? "yes" : "no") << endl;
        // cout << "Is optimal? " << ((cc_return == DISCORDE_RETURN_OK && cc_status == DISCORDE_STATUS_OPTIMAL) ? "yes" : "no") << endl << endl;


        vector<vector<uint64_t>> tours; 
        vector<uint64_t> aux(instance.num_nodes);
        vector<uint64_t> aux2;


        for(uint64_t i=0; i < instance.num_nodes; i++)
            aux[i] = cc_tour[i];
        
        tours.push_back(aux);

        for(uint64_t i=0; i < instance.num_nodes; i++)
            aux[i] = lk_tour[i];

        tours.push_back(aux);

        aux.assign(1, 0); aux.insert(aux.end(), tours[1].rbegin(), tours[1].rend()-1);

        if(mergesort_countinv(tours[0], tours[1]) > mergesort_countinv(tours[0], aux))
            tours[1] = aux;
   

        double alpha = 1/instance.drone_vel;

        for(int i=0; i<2; i++){
            for(double d=0.5; d<2.1; d+=0.25){
                if(alpha*d > 1){
                    aux = tours[i];
                    bolhas_elipticas_diretas(aux, instance, d);
                    tours.push_back(aux);

                    aux = tours[i];
                    bolhas_elipticas_reversas(aux, instance, d);
                    tours.push_back(aux);

                    aux.assign(1, 0); aux.insert(aux.end(), tours[i].rbegin(), tours[i].rend()-1);
                    bolhas_elipticas_diretas(aux, instance, d);
                    aux2.assign(1, 0); aux2.insert(aux2.end(), aux.rbegin(), aux.rend()-1);
                    tours.push_back(aux2);
                
                    aux.assign(1, 0); aux.insert(aux.end(), tours[i].rbegin(), tours[i].rend()-1);
                    bolhas_elipticas_reversas(aux, instance, d);
                    aux2.assign(1, 0); aux2.insert(aux2.end(), aux.rbegin(), aux.rend()-1);
                    tours.push_back(aux2);
                }
            }
        }        
        
        vector<BRKGA::Chromosome> chromosomes;
        std::mt19937 rng(seed);
        
        for(uint64_t i=0; i<tours.size(); i++){

            vector<double> keys(instance.num_nodes);
            for(auto& key : keys)
                key = generate_canonical<double, numeric_limits<double>::digits>(rng);
            

            sort(keys.begin(), keys.end());

            // Then, we visit each node in the tour and assign to it a key.
            BRKGA::Chromosome chromosome(instance.num_nodes);

            for(size_t j = 0; j < keys.size(); j++)
                chromosome[tours[i][j]] = keys[j];

            best = min(best, decoder.decode(chromosome, true));

            chromosomes.push_back(chromosome);
        }

        switch(method){
            case 0: 
                break;
            case 1: algorithm.setInitialPopulation(vector<BRKGA::Chromosome>(0));
                break;
            case 2: algorithm.setInitialPopulation(vector<BRKGA::Chromosome>(chromosomes.begin(), chromosomes.end()));
               break;
            default:
                break;
        }
        
        for (uint64_t i = 0; i < instance.num_nodes; ++i) {
            delete[] cost_matrix[i];
        }
        delete[] cost_matrix;
        delete[] lk_tour;
        delete[] cc_tour;
		
        } // end local scope

        auto end = chrono::high_resolution_clock::now();

        const auto final_status = algorithm.run(control_params, &cout);

        std::chrono::duration<double> elapsed_seconds = end - start;
        cout<<"initialization time: "<<elapsed_seconds.count()<<endl;
        cout<<"best warm start solution: "<<best<<endl;


        vector<pair<double, unsigned>> tour(instance.getN());
        for(unsigned i = 0; i < instance.getN(); ++i)
            tour[i] = make_pair(final_status.best_chromosome[i], i);

        // for(unsigned i = 0; i < instance.getN(); ++i)
        //     cout<<"("<<tour[i].first<< ", " <<tour[i].second<<") ";
        // cout<<endl;

        sort(tour.begin()+1, tour.end());

        // for(unsigned i = 0; i < instance.getN(); ++i)
        //     cout<<"("<<tour[i].first<< ", " <<tour[i].second<<") ";
        // cout<<endl;

        vector<int> permutation, predecessor;
        Digrafo graph(instance.getN());
        cout<<"TSP tour:\n";
        for(auto i : tour) {
            cout<<i.second<<" ";
            permutation.push_back(i.second);
        }
        cout<<endl;
        // cout<<"permutation:\n";
        // for(int i=0; i<instance.getN(); i++)
        //     cout<<permutation[i]<<" ";
        // cout<<endl;

        // cout<<endl;
        // double check = decoder.split_lazy(instance, permutation);
        // cout<<"valor do best_chormossome = "<<check<<endl;
        // check = decoder.split_lazy(instance, permutation, predecessor, graph);
        // cout<<"valor do best_chormossome2 = "<<check<<endl;
        decoder.split_lazy(instance, permutation, predecessor, graph);


        
        // cout<<"digrafo auxiliar:\n";
        // cout<<"(dest, cost, drone_node)\n";
        // for(int i=0; i<permutation.size(); i++){
        //     cout<<permutation[i]<<": ";
        //     for(arc *a = graph.nodes[permutation[i]]; a != nullptr; a = a->prox){
        //         cout<<"("<<a->dest<<", "<<a->cost<<", "<<a->drone_node<<"), ";
        //     }
        //     cout<<endl; 
        // }

        // cout<<"Predecessor:\n";
        // for(int i=0; i<permutation.size(); i++){
        //     cout<<permutation[i]<<": "<<predecessor[permutation[i]]<<endl;
        // }
        // cout<<instance.getN()<<": "<<predecessor[instance.getN()]<<endl;

        vector<pair<arc*,int>> final_tour;
        int ant = instance.getN();
        for(int i=predecessor[ant]; i>=0; i = predecessor[i]){
            arc *a = graph.nodes[i];
            arc* min_arc = a;
            while(a!=nullptr){

                if((a->dest == ant && a->cost < min_arc->cost) || (a->dest == ant && min_arc->dest != ant)){
                    min_arc = a;
                }
                
                a = a->prox;
            }
            final_tour.push_back(make_pair(min_arc, i));
            ant = i;
        }

        cout<<"Operations:"<<endl;

        for(int i=(int)final_tour.size()-1; i>=0; i--){
            cout<<final_tour[i].second<<","<<final_tour[i].first->drone_node<<","<<(final_tour[i].first->dest==instance.num_nodes?0:final_tour[i].first->dest)<<"|";
        }
        cout<<endl;


        cout
        << "method: " << method_name[method]
        << "\nseed: " << seed 
        << "\nInstance: "<< argv[4]
        << final_status
        << endl;
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

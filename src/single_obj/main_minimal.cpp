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

#include "tsp/tsp_instance.hpp"
#include "decoders/tsp_decoder.hpp"
#include "decoders/Tspdproblem.h"
#include "decoders/Tspdsolver.h"
#include "decoders/Digrafo.h"
#include "brkga_mp_ipr.hpp"
#include "heuristics/greedy_tour.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <random>

using namespace std;

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
			 
		
		
		
		//auto initial_solution = greedy_tour(instance);
		//const auto chromosome_size = instance.num_nodes;
		
		
		{ // local scope to deallocate memory.
        // To inject the initial tour, we need to create chromosome representing
        // that solution. First, we create a set of keys to be used in the
        // chromosome.
		
        // std::mt19937 rng(seed);
        // vector<double> keys(chromosome_size);
        // for(auto& key : keys)
        //     key = generate_canonical<double, numeric_limits<double>::digits>(rng);

        // sort(keys.begin(), keys.end());

        // // Then, we visit each node in the tour and assign to it a key.
        // BRKGA::Chromosome initial_chromosome(chromosome_size);
        // auto& initial_tour = initial_solution.second;
        // for(size_t i = 0; i < keys.size(); i++)
        //     initial_chromosome[initial_tour[i]] = keys[i];


        switch(method){
            case 0: 
                break;
            case 1: algorithm.setInitialPopulation(vector<BRKGA::Chromosome>(0));
                break;
            //está comentado porque o warm start ainda não está funcionando
            //case 2: algorithm.setInitialPopulation(vector<BRKGA::Chromosome>(1, inital_chromosome));
               //break;
            default:
                break;
        }
		
        } // end local scope

        const auto final_status = algorithm.run(control_params, &cout);

        vector<pair<double, unsigned>> tour(instance.getN());
        for(unsigned i = 0; i < instance.getN(); ++i)
            tour[i] = make_pair(final_status.best_chromosome[i], i);

        sort(tour.begin()+1, tour.end());
        vector<int> permutation, predecessor;
        Digrafo graph(instance.getN());
        cout<<"TSP tour:\n";
        for(auto i : tour) {
            cout<<i.second<<" ";
            permutation.push_back(i.second);
        }
        cout<<endl;
        decoder.split_lazy(instance, permutation, predecessor, graph);

        vector<pair<arc*,int>> final_tour;
        int ant = instance.getN();
        for(int i=predecessor[ant]; i>=0; i = predecessor[i]){
            arc *a = graph.nodes[i];
            final_tour.push_back(make_pair(a, i));
            int len = final_tour.size()-1;
            while(a->prox!=nullptr){

                if((a->dest == ant && a->cost < final_tour[len].first->cost) || (a->dest == ant && final_tour[len].first->dest != ant)){
                    final_tour[len] = make_pair(a, i);
                }
                
                a = a->prox;
            }
            ant = i;
        }

        cout<<"Operations:"<<endl;

        for(int i=(int)final_tour.size()-1; i>=0; i--){
            cout<<final_tour[i].second<<","<<final_tour[i].first->drone_node<<","<<(final_tour[i].first->dest==100?0:final_tour[i].first->dest)<<"|";
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

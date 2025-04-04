/******************************************************************************
 * tsp_instance.cpp: Implementation for TSP_Instance class.
 *
 * Created on : Mar 05, 2019 by andrade
 * Last update: Mar 05, 2019 by andrade
 *
 * (c) Copyright 2015-2019, Carlos Eduardo de Andrade.
 * All Rights Reserved.
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

#include <fstream>
#include <stdexcept>
#include <cmath>
#include <iostream>

using namespace std;

//-----------------------------[ Constructor ]--------------------------------//

TSP_Instance::TSP_Instance(const std::string& filename):
    truck_vel(0),
    drone_vel(0),
    num_nodes(0),
    distances(),
    positions()
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
}

//-------------------------------[ Distance ]---------------------------------//

// double TSP_Instance::distance(unsigned i, unsigned j) const {
//     // cout<<i<<" "<<j<<" ";
//     // if(i==j) return 0;
//     // if(i<j) swap(i,j);
//     // cout<<(((i)*(i-1))>>1)+j<<endl;
//     return distances[i*num_nodes + j];
// }

double TSP_Instance::distance(unsigned i, unsigned j) const {
    double aux;
    aux = pow((positions[i*2]-positions[j*2]),2);
    aux += pow((positions[i*2 +1]-positions[j*2 +1]),2);
    return sqrt(aux);
}

void TSP_Instance::distance_calc(){
    double aux;
    for(long unsigned i=0; i<num_nodes; i++){
        // cout<<i<<": ";
        for(long unsigned j=0; j<i; j++){
            aux = pow((positions[i*2]-positions[j*2]),2);
            aux += pow((positions[i*2 + 1]-positions[j*2 + 1]),2);
            aux = sqrt(aux);
            distances[i*num_nodes + j] = aux;
            distances[j*num_nodes + i] = aux;
            // cout<<distances[(((i)*(i-1))>>1)+j]<<" ";
        }
        // cout<<endl;
    }
    // cout<<endl;

    return;   
}

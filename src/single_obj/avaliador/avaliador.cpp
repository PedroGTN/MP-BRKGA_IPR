#include "avaliador.hpp"

double avaliador::avaliar(){
    cout<<"1\n";

    Tspd_solver decoder(this->instance);
    cout<<"2\n";
    vector<int> predecessor;
    cout<<"3\n";
    Digrafo graph(this->instance.getN());

    cout<<"4\n";
    this->tour_value = decoder.split_lazy(this->instance, this->og_tour, predecessor, graph);


    vector<pair<arc*,int>> final_tour;
    int ant = this->instance.getN();
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
    cout<<"5\n";
    for(int i=(int)final_tour.size()-1; i>=0; i--){
        this->drone_operations[i].origin = final_tour[i].second;
        this->drone_operations[i].drone_node = final_tour[i].first->drone_node;
        this->drone_operations[i].rendezvous = final_tour[i].first->dest==this->instance.num_nodes?0:final_tour[i].first->dest;
    }
}

bool avaliador::verificar(){
    bool flag = this->og_tour_value == this->tour_value;

    flag = og_tour[0] == 0;

    vector<int> nodes(this->instance.getN(), 0);

    for(int i=0; i<this->instance.getN(); i++){
        nodes[og_tour[i]] += 1;
        flag = nodes[og_tour[i]] < 2;
    }

    return flag;
}
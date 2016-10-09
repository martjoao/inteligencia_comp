#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>


using namespace std;

int main() {
    Instance *i = Instance::parseInstance("instances/instance1.txt");
    cout << "\n\n" << endl;
    cout << "Num. Gateways: \t" <<  i->nGateways << endl;
    cout << "Num. Clients: \t"  <<  i->nClients << endl;
    cout << "Num. Groups: \t"   <<  i->nGroups << endl;
    cout << "Gate Capacity: \t"   <<  i->gCapacity << endl << endl;

    for (int j =0; j < i->nClients; j++) {
        cout << i->clientBandwidth[j] << " ";
    }
    cout << endl << endl;

    for (int j = 0; j < i->nGateways; j++) {
        for (int k = 0; k < i->gRanges[j].size(); k++) {
            cout << i->gRanges[j][k] << " ";
        }
        cout << endl;
    }

    cout << endl;
    
    for (int j = 0; j < i->nGroups; j++) {
        for (int k = 0; k < i->groups[j].size(); k++) {
            cout << i->groups[j][k] << " ";
        }
        cout << endl;
    }

    //Agora precisamos pegar os groups e colocar eles nos Gateways.
    /*Tenho uma matriz de Groups. Vou passar essa matriz para um conjunto de objetos da classe group*/
    vector<Group> grps;
    for(int j = 0; j < i->nGroups; j++){
        grps.push_back(*(new Group(&(i->groups[j]), j)));
    }

    //Pronto, todos os grupos já estão na estrutura de dados grps
    //Agora vou começar a alocar os grupos nos gateways

    /*SOLUÇÃO GULOSA*/
    vector<int> occGateways(i->nGateways, 0);
    vector<int>  gateCapacities(i->nGateways, i->gCapacity);
    int foo = 0;
    int nDummy = 0;
    for(int j = 0; j < i->nGroups; j++){
        //Cada grupo vai ser sorteado para um gateway aleatório
        srand(j + time(NULL) + foo);
        int sGateway = rand() % i->nGateways;
        //É bom que cada grupo seja selecionado para um gateway distinto
        if(occGateways[sGateway] == 1 && 
            find(occGateways.begin(), occGateways.end(), 0) != occGateways.end()){
                //Tenho que tentar sortear outro gateway. Esse está ocupado e ainda existem gateways desocupados
                foo++;
                j--;
                continue;
            }
        else{
            occGateways[sGateway] = 1;
            //Agora vou tentar  colocar todos os clientes desse grupo nesse gateway
            int capacity = gateCapacities[sGateway];
            for(int k = 0; k < grps[j].clients->size(); k++){
                int clientDemand = i->clientBandwidth[grps[j].clients->at(k)];
               if(capacity < clientDemand){
                   grps[j].setGateway(k, DUMMY);
               } //Cliente vai para o gateway dummy
               else{
                   capacity -= clientDemand;
                   grps[j].setGateway(k, sGateway);
               }
            }
        }
    }

    /*Finalmente, temos que computar o custo dessa solucao*/
    

}

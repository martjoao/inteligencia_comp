#include "../include/group.h"
#include "../include/instance.h"
#include <vector>
#include <list>
#include <unordered_map>


Group::Group(vector<int> *group, int indexGroup){
    //First we must allocate the adj matrix that will represent the group
    nClients = group->size();
    clients = group;
    clientGateway = vector<int>(nClients, -1);
}

int Group::getGateway(int nclient){

    return clientGateway[nclient];
}

void Group::setGateway(int client, int gateway){
    clientGateway[client] = gateway;
}


int Group::nCliques(){
    vector<int> distInts;
    /*Number of cliques is the number of distinct integers in this list*/
    for(int i = 0; i < clientGateway.size(); i++){
        int nGate = clientGateway[i];
        bool foundGate = false;        
        if(distInts.empty()) distInts.push_back(nGate);

        for(int j = 0; j < distInts.size(); i++){
            if (nGate == distInts[j]) foundGate = true;
        }
        if(!foundGate) distInts.push_back(nGate);
    }
    return distInts.size();
}

float Group::cost(){//Float if we consider the distances
    unordered_map<int, int> freq(0);

    for (int i = 0; i < clientGateway.size(); i++) {
        freq[clientGateway[i]]++;
    }

    int cost = 0;
    for (auto k : freq) {
        if (k.first == DUMMY) {
            cost += (2000 * k.second);
        }
        cost += k.second * (clientGateway.size() - k.second);
    }
    return cost/2;


}

// float Group::cost(){//Float if we consider the distances
//     /*Will compute the cost from a group*/
//     vector< list<int> > graph(nClients);
//     //graph.resize(clients->size());
//     /*Each list inside the list represents a clique*/

//     //First we have to build this graph

//     for(int i = 0; i < clientGateway.size(); i++){
//         int nGate = clientGateway[i];
//         graph[nGate].push_back(i);
//     }

//     float grpCost = 0;
//     for(int i = 0; i < graph.size(); i++){
//         //Now we compute grpCost
//         grpCost += graph[i].size() * (clients->size() - graph[i].size());
//     }
//     return grpCost/2;
//     /*grpCost sempre vai ser divisivel por 2*/
// }

Group::~Group(){

}
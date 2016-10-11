#include "../include/group.h"
#include "../include/instance.h"
#include <vector>
#include <list>
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
    /*Will compute the cost from a group*/
    vector< list<int> > graph;
    graph.resize(clients->size());
    /*Each list inside the list represents a clique*/

    //First we have to build this graph

    for(int i = 0; i < clientGateway.size(); i++){
        int nGate = clientGateway[i];
        graph[i].push_back(i);
    }

    float grpCost = 0;
    for(int i = 0; i < graph.size(); i++){
        //Now we compute grpCost
        grpCost += graph[i].size() * (clients->size() - graph[i].size());
    }
    return grpCost/2;
    /*grpCost sempre vai ser divisivel por 2*/
}

Group::~Group(){

}
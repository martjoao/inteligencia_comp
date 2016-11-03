#include <iostream>
#include <stdlib.h>
#include "instance.h"
#include <time.h>
#include <algorithm>
#include <map>


#define DUMMY 999

using namespace std;

void dumpInstance(const Instance *i) {
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

    cout << endl << endl << "ADJ MAT:" << endl;
 
    for (int j = 0; j < i->adjMat->size(); j++) {
        for (int k = 0; k < i->adjMat->size(); k++) {
            cout << (*(i->adjMat))[j][k] << " ";
        }
        cout << endl;
    }


    cout << "CLIENTS IN GROUPS: " << endl;
    for (int j =0; j < i->nClients; j++) {
        cout << i->clientGroup[j] << " ";
    }
    cout << endl << endl;

}

void dumpSol(Instance * inst, std::vector<int>& solution) {

    std::cout << "SOLUTION: " << std::endl;
    for (int i = 0; i < inst->nClients; i++) {
        std::cout << solution[i] << " ";
    }
    std::cout << std::endl;

}



int main() {

    //Config environment
    srand(time(NULL));

    Instance *inst = Instance::parseInstance("instances/instance2.txt");
    dumpInstance(inst);


    /*
        Solução:
            Array de tamanho nClients
            Array[i] = gateway de I;
            se i == nClients -> DUMMY
    */


    int usedGateways = 0;

    //solution data structure:
    std::vector<int> solution(inst->nClients, -1);

    std::map<int, int> loads;    //maps gateway to its current load
    std::map<int, std::vector<int> > prefGateways; //maps group to gateways

    
    //Do I need this?
    for (int i = 0; i < inst->nGateways; i++) {
        loads[i] = 0;        
    }



    //for each group, add all clients to a single gateway if possible
    for (int i = 0; i < inst->nGroups; i++) {      

        std::cout << "Alocating group " << i << std::endl;
        dumpSol(inst, solution);

        //select a gateway for group i
        int g;


        if (usedGateways < inst->nGateways) {
            do {
                g = rand() % inst->nGateways;
            } while (loads[g] != 0);
            usedGateways++;
        }
        else {
            g = DUMMY;
        }
       
        prefGateways[i].push_back(g);
        for (int j = 0; j < inst->groups[i].size(); j++) {
            if (g!=DUMMY && loads[g] + inst->clientBandwidth[inst->groups[i][j]] <= inst->gCapacity) {
                solution[inst->groups[i][j]] = g;
                loads[g] += inst->clientBandwidth[inst->groups[i][j]];
            }
            else {
                solution[inst->groups[i][j]] = DUMMY;
            }
        }

        std::cout << "Alocated group " << i << std::endl;
        dumpSol(inst, solution);
        
    }
    dumpSol(inst, solution);
}

#include <iostream>

#include "instance.h"

using namespace std;

int main() {
    Instance *i = Instance::parseInstance("instances/instance1.txt");

    cout << i->nGateways << endl;
    cout << i->nClients << endl;
    cout << i->nGroups << endl;
    cout << i->gCapacity << endl << endl;

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

}

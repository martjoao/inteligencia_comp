#include "instance.h"

#include <fstream>
#include <string>
#include <sstream>

using namespace std;


Instance::Instance() {}


Instance* Instance::parseInstance(string filename) {

    Instance* i = new Instance();
    ifstream ifs(filename.c_str());

    //read quantities
    ifs >> i->nGateways >> i->nClients >> i->nGroups >> i->gCapacity;
   
    //read requested bandwidths

    for (int j = 0; j < i->nClients; j++) {
        int cli;
        ifs >> cli;
        i->clientBandwidth.push_back(cli);    
    }


    int n;
    string line;
    getline(ifs, line);
    getline(ifs, line);

    //read ranges
    for (int j = 0; j < i->nGateways; j++) {
        i->gRanges.push_back(vector<int>());

        getline(ifs, line);
        istringstream iss(line);
    
        while (iss >> n) {
            i->gRanges[j].push_back(n);
        }
    }    

    getline(ifs, line);

    //read groups
    for (int j = 0; j < i->nGroups; j++) {
        i->groups.push_back(vector<int>());
             
        getline(ifs, line);
        istringstream iss(line);
         
        while (iss >> n) {
            i->groups[j].push_back(n);
        }   
    }
//*/

    ifs.close();


    return i;

}
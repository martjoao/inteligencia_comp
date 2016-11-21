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
        i->clientGroup.push_back(-1);    
    }


    int n;
    string line;
    getline(ifs, line);
    getline(ifs, line);

    /*
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
    //*/   

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


    i->adjMat = new vector<vector<int> >(i->nClients , vector<int>(i->nClients , 0));
    
    for (int j = 0; j < i->nGroups; j++) {
        for (int k = 0; k < i->groups[j].size(); k++) {
            for (int l = k; l < i->groups[j].size(); l++) {
                int m = i->groups[j][k] ;
                int n = i->groups[j][l] ;
                (*(i->adjMat))[m][n] = (*(i->adjMat))[n][m] = 1;
            }
        }
    }

    ifs.close();

    for (int j = 0; j < i->nGroups; j++) {
        
        for (int k = 0; k < i->groups[j].size(); k++) {
            i->clientGroup[i->groups[j][k]] = j;
        }
    }

    return i;

}

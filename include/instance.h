#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>

using namespace std;

class Instance {
    public:
        static Instance* parseInstance(string filename);

        int nGateways, nClients, nGroups, gCapacity;
        vector<int> clientBandwidth;
        vector<vector<int> > gRanges;
        vector<vector<int> > groups;
        vector<vector<int> >* adjMat;



    private:
        Instance();
};

#endif

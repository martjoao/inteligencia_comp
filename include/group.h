#ifndef GROUP_H
#define GROUP_H
#include <vector>
#include "instance.h"

#define DUMMY -999


using namespace std;
class Group{
    public:
        vector<int> *clients;
        vector<int> clientGateway;
        Group(vector<int> *group, int index);
        ~Group();
        int nCliques();
        void setGateway(int client, int Gateway);
        int getGateway(int client);
        float cost();
        int nClients;
};



#endif
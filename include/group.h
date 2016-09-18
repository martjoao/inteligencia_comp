#ifndef GROUP_H
#define GROUP_H
#include <vector>
#include "instance.h"

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
};



#endif
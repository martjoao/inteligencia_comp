#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>

using namespace std;

bool localSearch(vector<Group> &newGrps, vector<int> &gateCapacities, int *solCost, Instance *inst);
bool localSearchCostOPT(vector<Group> &newGrps, vector<int> &gateCapacities, int *solCost, Instance *inst);
void ILS(vector<Group> grp, vector<int> &gateCapacities, Instance *inst);
int calcCost(vector<Group> group);

#include <iostream>

#include "instance.h"

using namespace std;

int main() {
    Instance *i = Instance::parseInstance("instances/instance1.txt");

    cout << i->nGateways << endl;
}

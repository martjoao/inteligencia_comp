#include <iostream>

#include "instance"

using namespace std;

int main() {
    Instance *i = Instance::parseInstance("instances/instance1.txt");

    cout << i->nGateways << endl;
}

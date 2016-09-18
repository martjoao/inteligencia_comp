#include "instance.h"

#include <fstream>
using namespace std;

Instance::Instance() {}

Instance* Instance::parseInstance(string filename) {

    Instance* i = new Instance();

    fstream fs;
    fs.open(filename, fstream::in);

    fs >> i->nGateways;

    fs.close();

    return i;

}

#include "instance.h"

#include <fstream>
using namespace std;

Instance::Instance() {}

Instance* Instance::parseInstance(string filename) {

    Instance* i = new Instance();

    ifstream ifs(filename.c_str());

    ifs >> i->nGateways;

    ifs.close();


    return i;

}

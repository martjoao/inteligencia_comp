#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

void printUsage() {
		cout << "USAGE:\n./gen.out <Number of gateways> <Gateway Capacity> <number of groups> <epsilon> <output file>" << endl;
}

int main(int argc, char** argv) {

		//parsing inputs
		if (argc != 6) {
			printUsage();
			return -1;
		}

		int nGateways = 0, capacity = 0, nGroups = 0, epsilon = 0;
		string filename;

		try {
			nGateways = stoi(string(argv[1]));
			capacity  = stoi(string(argv[2]));
			nGroups   = stoi(string(argv[3]));
			epsilon   = stoi(string(argv[4]));
			filename = string(argv[5]);
			
		} 
		catch(invalid_argument& e){
			printUsage();
			return -1;
		}

		//generate clients -> client i has clients[i] load
		
		srand(time(NULL));
		vector<int> clients;
		vector<int> groups;

		for (int i = 0; i < nGateways; i++) {
		    int currentLoad = 0;
		    while (currentLoad < capacity - epsilon) {
		    		int load = (rand() % (capacity - currentLoad - 1)) + 1;
		    		load %= capacity/4;
		    		load++;
		    		clients.push_back(load);
		    		currentLoad += load;
		    }
		}

		//set client's groups
		for (int i = 0; i < clients.size(); i++) {
				groups.push_back(rand() % nGroups);
		}


		//output
		ofstream outfile;
   	outfile.open(filename);

   	outfile << 
   		nGateways << endl << 
   		clients.size() << endl 
   		<< nGroups << endl
   		<< endl 
   		<< capacity << endl
   		<< endl;

   	for (int i = 0; i < clients.size(); i++) {
   		outfile << clients[i] << " ";
   	}
   	outfile << endl << endl;

   	//O(n2), mudar depois, to com pressa
   	for (int g = 0; g < nGroups; g++) {
		   	for (int i = 0; i < clients.size(); i++) {
		   		  if (g == groups[i]) {
		   		  		outfile << i << " ";
		   		  }
		   	}
		   	outfile << endl;
   	}

   	outfile.close();
}
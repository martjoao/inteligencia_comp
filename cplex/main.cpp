#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>
#include <ilcplex/ilocplex.h>
#include <sys/time.h>

double Optimize(Instance* inst)
{
    char var[100];

		//Ambiente
    IloEnv env;

    // Criando um modelo
    IloModel model(env);


    // Variáveis de decisão

    //Yik = 1 se i(V) pertence a k(G), 0 c.c. 
    IloArray <IloBoolVarArray > y(env, inst->nClients);
    for (int i = 0; i < inst->nClients; ++i) {
        IloBoolVarArray vec(env, inst->nGateways);
        y[i] = vec;
    }
    //IloNumVarArray vetor(env, M, 0, 1);

    // Adicionando a variável y no modelo
    for (int i = 0; i < inst->nClients; i++) {
        for (int k = 0; k < inst->nGateways; k++) {
            sprintf(var, "Y(%d,%d)", i, k);
            y[i][k].setName(var);
            model.add(y[i][k]);
        }
    }

    // Creating x variables (3 indices)
    // Xijk = 1 se i,j sao do mesmo grupo e estão no 
    //mesmo gateway, 0 c.c. 
    IloArray <IloArray <IloBoolVarArray> > x(env, inst->nClients);

    for (int i = 0; i < inst->nClients; i++) {
      	IloArray <IloBoolVarArray> mat(env, inst->nClients);
      	x[i] = mat;
    }

    for (int i = 0; i <= inst->nClients; i++) {
        for (int j = 0; j <= inst->nClients; j++) {
            IloBoolVarArray vec (env, inst->nGateways);
            x[i][j] = vec;
        }
    }

    // Adding x variables to the model
    for (int i = 0; i < inst->nClients; i++) {
        for (int j = 0; j < inst->nClients; j++) {
            for (int k = 0; k < inst->nGateways; k++) {
	            sprintf(var, "X(%d,%d,%d)", i, j, k);
	            x[i][j][k].setName(var);
	            model.add(x[i][j][k]);
            }
        }
    }


  	IloExpr objFunc(env);
  	for (int i = 0; i < inst->nClients; i++) {
		for (int j = 0; j < inst->nClients; j++) {
			for (int k = 0; k < inst->nGateways; k++) {
				objFunc += x[i][j][k];
			}
		}
	}

    // Adicionando a FO
    model.add(IloMaximize(env, objFunc));


    //RESTRICAO 1
    for (int k = 0; k < inst->nGateways; k++) {
		IloExpr rest(env);
		for (int i = 0; i < inst->nClients; i++) {
			rest += inst->clientBandwidth[i] * y[i][k];
		}
		model.add(rest <= inst->gCapacity);
    }

    //RESTRICAO 2 
	for (int i = 0; i < inst->nClients; i++) {
		for (int j = 0; j < inst->nClients; j++) {
			for (int k = 0; k < inst->nGateways; k++) {
				//if (inst->clientGroup[i] == inst->clientGroup[j])
				model.add(y[i][j] >= x[i][j][k]);
			}
		}
	}

	//RESTRICAO 3
    for (int i = 0; i < inst->nClients; i++) {
		IloExpr rest(env);
		for (int k = 0; k < inst->nGateways; k++) {
			rest += y[i][k];
		}
		model.add(rest == 1);
    }



    IloCplex TESTE(model);
    //CLAW.exportModel("CLAW.lp");
    TESTE.setParam(IloCplex::Threads, 1);
    TESTE.solve();

    double cost = TESTE.getObjValue();
    cout << "OBJ: " << cost << endl;

    for (int i = 0; i < inst->nClients; i++)
    {
        for (int k = 0; k < inst->nGateways; k++) {
            cout << y[i][k] << " ";
        }
        cout << endl;
    }

    return cost;
}


int main(int argc, char** argv)
{
	struct timeval tempoAntes;
    struct timeval tempoDepois;
	srand(time(NULL));

    Instance *inst = Instance::parseInstance("../instances/instance2.txt");

	gettimeofday(&tempoAntes, NULL);
	Optimize(inst);
	gettimeofday(&tempoDepois, NULL);
	 
	timeval dif;
	long tempo;
  
    timersub(&tempoDepois, &tempoAntes, &dif);
	
	tempo = dif.tv_sec*1000;
    
    if(dif.tv_usec != 0)
		tempo += dif.tv_usec/1000;

	std::cout << "FINAL DA EXECUÇÃO - IMPRIMIR DADOS" << std::endl;
}

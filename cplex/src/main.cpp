#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>

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
    for (int i = 0; i < n; ++i) {
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

    // Adicionando a FO
    modelo.add(IloMinimize(env, PosEdge + NegEdge));


    // Restricoes 1 - Proibi a formação de P4
    for (int i = 0; i < n; i++)
    {
       for (int j = i+1; j < n; j++)
       {
          for (int k = j+1; k < n; k++)
          {
         for (int l = k+1; l < n; l++)
         {
            modelo.add( x[i][j] + x[i][k] + x[i][l] <= x[j][k] + x[j][l] + x[k][l] + 2);
            modelo.add( x[i][j] + x[j][k] + x[j][l] <= x[i][k] + x[i][l] + x[k][l] + 2);
            modelo.add( x[i][k] + x[j][k] + x[k][l] <= x[i][j] + x[i][l] + x[j][l] + 2);
            modelo.add( x[i][l] + x[j][l] + x[k][l] <= x[i][j] + x[i][k] + x[j][k] + 2);
         }
          }
       }
    }

    IloCplex CLAW(modelo);
    //CLAW.exportModel("CLAW.lp");
    CLAW.setParam(IloCplex::Threads, 1);
    CLAW.solve();

    cout << "Nodes: " << CLAW.getNnodes() << endl;
    double cost = CLAW.getObjValue();
    cout << "OBJ: " << cost << endl;

    for (int i = 0; i < n; i++)
    {
       for (int j = i+1; j < n; j++)
       {
          if((CLAW.getValue(x[i][j]) < 0.01) && (w[i][j] == 1))
            cout << "DELETE I: " << i << " J: " <<j << endl;
          if((CLAW.getValue(x[i][j]) > 0.99) && (w[i][j] == 0))
            cout << "ADD I: " << i << " J: " <<j << endl;
       }
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
	
	tempo = dif.tv_sec*1000
  if(dif.tv_usec != 0)
		tempo += dif.tv_usec/1000;

	std::cout << "FINAL DA EXECUÇÃO - IMPRIMIR DADOS" << std::endl;
}

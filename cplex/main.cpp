#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>

double Optimize(int n, int **w, Instance* inst)
{
    IloEnv env;

    // Criando um modelo
    IloModel modelo(env);

    // Variáveis de decisão

    // Variável x
    IloArray <IloBoolVarArray > y(env, inst->nClients);
    //IloArray <IloNumVarArray > x(env, P);

    for (int i = 0; i < n; ++i) {
        IloBoolVarArray vetor(env, n);
        //IloNumVarArray vetor(env, M, 0, 1);
        y[i] = vetor;
    }

    // Adicionando a variável x no modelo
     char var[100];k
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < inst->nGateways; k++) {
                sprintf(var, "Y(%d,%d)", i, k);
                y[i][k].setName(var);
                modelo.add(y[i][k]);
            }
        }

        // Adicionando a FO
        modelo.add(IloMinimize(env, PosEdge + NegEdge));





     // Creating w variables (3 indices)

    IloArray <IloArray <IloBoolVarArray> > w(env, N+1);

    for (int i = 0; i <= N; i++) {
      IloArray <IloBoolVarArray> matriz(env, N+1);
      w[i] = matriz;
    }

    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            IloBoolVarArray vetor (env, Q+1);
            w[i][j] = vetor;
        }
    }

    // Adding w variables to the model

    for (int i = 1; i < N; i++) {
        for (int j = 1; j < N; j++) {
            for (int k = 1; k < Q; k++) {
                if (i != j && k <= draft[j]) {
                    sprintf(var, "W(%d,%d,%d)", i, j, k);
                    w[i][j][k].setName(var);
                    model.add(w[i][j][k]);
                }
            }
        }
    }





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

int main() {
    Instance *inst = Instance::parseInstance("instances/instance2.txt");
    //dumpInstance(inst);
}
#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>
#include "localSearch.h"
#include <ctime>

using namespace std;

/*double OptimizeClaw(int n, int **w, Instance* inst)
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
}*/



int main() {
    Instance *i = Instance::parseInstance("instances/instance-medium-3.txt");
    //Instance *i = Instance::parseInstance("instances/instance2.txt");
    cout << "\n\n" << endl;
    cout << "Num. Gateways: \t" <<  i->nGateways << endl;
    cout << "Num. Clients: \t"  <<  i->nClients << endl;
    cout << "Num. Groups: \t"   <<  i->nGroups << endl;
    cout << "Gate Capacity: \t"   <<  i->gCapacity << endl << endl;

    /*for (int j =0; j < i->nClients; j++) {
        cout << i->clientBandwidth[j] << " ";
    }
    cout << endl << endl;

    for (int j = 0; j < i->nGateways; j++) {
        for (int k = 0; k < i->gRanges[j].size(); k++) {
            cout << i->gRanges[j][k] << " ";
        }
        cout << endl;
    }*/

    //cout << endl;

    /*for (int j = 0; j < i->nGroups; j++) {
        for (int k = 0; k < i->groups[j].size(); k++) {
            cout << i->groups[j][k] << " ";
        }
        cout << endl;
    }

    cout << endl << endl << "ADJ MAT:" << endl;

    for (int j = 0; j < i->adjMat->size(); j++) {
        for (int k = 0; k < i->adjMat->size(); k++) {
            cout << (*(i->adjMat))[j][k] << " ";
        }
        cout << endl;
    }*/


    //Agora precisamos pegar os groups e colocar eles nos Gateways.

    /*Tenho uma matriz de Groups. Vou passar essa matriz para um conjunto de objetos da classe group*/
    vector<Group> grps;
    for(int j = 0; j < i->nGroups; j++){
        grps.push_back(*(new Group(&(i->groups[j]), j)));
    }

    //Pronto, todos os grupos já estão na estrutura de dados grps
    //Agora vou começar a alocar os grupos nos gateways

    /*SOLUÇÃO GULOSA*/
    vector<int> occGateways(i->nGateways, 0);
    vector<int> gateCapacities(i->nGateways, i->gCapacity);

    int foo = 0;
    int nDummy = 0;

    for(int j = 0; j < i->nGroups; j++){
        //Cada grupo vai ser sorteado para um gateway aleatório
        srand(j + time(NULL) + foo);
        int sGateway = rand() % i->nGateways;
        //É bom que cada grupo seja selecionado para um gateway distinto

        if(occGateways[sGateway] == 1 &&
            find(occGateways.begin(), occGateways.end(), 0) != occGateways.end()){
                //Tenho que tentar sortear outro gateway. Esse está ocupado e ainda existem gateways desocupados
                foo++;
                j--;
                continue;
            }
        else{
            occGateways[sGateway] = 1;
            //Agora vou tentar  colocar todos os clientes desse grupo nesse gateway
            int capacity = gateCapacities[sGateway];
            for(int k = 0; k < grps[j].clients->size(); k++){
               int clientDemand = i->clientBandwidth[grps[j].clients->at(k)];
               if(capacity < clientDemand){
                   grps[j].setGateway(k, DUMMY);
               } //Cliente vai para o gateway dummy
               else{
                   capacity -= clientDemand;
                   grps[j].setGateway(k, sGateway);
               }
            }
            gateCapacities[sGateway] = capacity;//Capacidade livre do gateway atualizada
        }
    }


    /*cout << endl << endl << "Greedy sol: " << endl;

    for (int k = 0; k < i->nGroups; k++) {
        cout << "Group " << k << endl;

        for (int l = 0; l < grps[k].nClients; l++) {
            cout << (*(grps[k].clients))[l] << " - " <<
                grps[k].clientGateway[l] << endl;
        }
    }*/

    /*Finalmente, temos que computar o custo dessa solucao*/
    int greedyCost = 0;
    /*for (int k = 0; k < i->nGroups; k++) {
        greedyCost += grps[k].cost();
    }
    cout << "Greedy cost: " << greedyCost << endl;*/



    /*LocalSearch OPT*/
    while(localSearchCostOPT(grps, gateCapacities, &greedyCost, i));//Does all the operations

    /*cout << "Solucao Otimizada" << endl;
    for (int k = 0; k < i->nGroups; k++) {
        cout << "Group " << k << endl;

        for (int l = 0; l < grps[k].nClients; l++) {
            cout << (*(grps[k].clients))[l] << " - " <<
                grps[k].clientGateway[l] << endl;
        }
    }*/
    int optCost =  calcCost(grps);
    cout << "First Solution cost : " << optCost << endl;
    //Vou imprimir a solução depois da busca local

    //Agora vamos otimizar usando o ILS
    clock_t begin = clock();
    ILS(grps, gateCapacities, &optCost, i);
    clock_t end = clock();

    double totalTime = double(end - begin)/CLOCKS_PER_SEC;
    cout << endl << "ILS total running time: " << totalTime << "s" << endl;

}

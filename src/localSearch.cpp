  #include "localSearch.h"
#include <iostream>
#include <stdlib.h>
#include "group.h"
#include "instance.h"
#include <time.h>
#include <algorithm>
#include "localSearch.h"
#include <fstream>

using namespace std;

int calcCost(vector<Group> grps){
    float solCost = 0;
     for (int k = 0; k < grps.size(); k++) {
        solCost += grps[k].cost();
    }
    return solCost;
}

int calcCostMax(vector<Group> grps){
    int solCost = 0;
     for (int k = 0; k < grps.size(); k++) {
        solCost += grps[k].costMax();
    }
    return solCost;
}

bool localSearch(vector<Group> &newGrps, vector<int> &gateCapacities, int *solCost, Instance *inst){
    int newCost = 999999;
    /*Agora vamos fazer um swap nos elementos entre os gateways*/
    /*for(int i = 0; i < newGrps.size(); i++){
        for(int j = i+1; j < newGrps.size();j++){
            //Vou trocar todos os elementos do grupo I com o grupo J e checar o custo de cada trocar
            Group *grp1 = &newGrps[i];
            Group *grp2 = &newGrps[j];
            //Agora vamos fazer um laço para percorrer os clientes de cada grupo
            for(int iFoo = 0; iFoo < grp1->nClients; iFoo++){
                for(int jFoo = 0; jFoo < grp2->nClients; jFoo++){
                    int clientDemand1 = inst->clientBandwidth[grp1->clients->at(iFoo)];
                    int clientDemand2 = inst->clientBandwidth[grp2->clients->at(jFoo)];
                    int gateIFoo = grp1->clientGateway[iFoo];
                    int gateJFoo = grp2->clientGateway[jFoo];
                    if( (gateCapacities[gateIFoo] + clientDemand1 - clientDemand2 < 0 ) ||
                        (gateCapacities[gateJFoo] + clientDemand2 - clientDemand1) < 0){
                        continue;//Esses clientes não podem ser trocados, isso vai estourar a capacidade dos gates
                    }
                    grp1->clientGateway[iFoo] = gateJFoo;
                    grp2->clientGateway[jFoo] = gateIFoo;
                    newCost = calcCost(newGrps);
                    if(newCost < *solCost){
                        *solCost = newCost;
                        gateCapacities[gateIFoo] += clientDemand1 - clientDemand2;
                        gateCapacities[gateJFoo] += clientDemand2 - clientDemand1;
                        return true;//Nova solucao :)
                    }
                    else{
                        grp1->clientGateway[iFoo] = gateIFoo;
                        grp2->clientGateway[jFoo] = gateJFoo;
                        //Volta ao estado original :]
                    }
                }
            }
        }
    }*/

    //Se essa otimizacao nao der certo, vamos tentar de outro jeito.
    //Vamos apenas trocar um cliente de um gateway para outro.
    //Para todos os clientes, vamos tentar todos os gateways
    for(int j = 0; j < newGrps.size(); j++){
      for(int k = 0; k < newGrps[j].clients->size(); k++){
        //Agora vou em todos os gateways e vou trocando os clientes
        int gateway = newGrps[j].clientGateway[k];
        for(int l = 0; l < inst->nGateways; l++){
          //Agora vamos trocando e calculando o custo. Vamos ver
          //O cliente K do grupo J vai para o gateway L
          int demand = inst->clientBandwidth[newGrps[j].clients->at(k)];
          if(demand > gateCapacities[l]) continue;
          else{
            newGrps[j].clientGateway[k] = l;
            newCost = calcCost(newGrps);
            if (newCost < *solCost ){
              *solCost = newCost;
              gateCapacities[l] -= demand;
              gateCapacities[gateway] += demand;
              return true;
            }else{
              newGrps[j].clientGateway[k] = gateway;
            }
          }
        }
      }
    }


    return false;
}


bool localSearchCostOPT(vector<Group> &newGrps, vector<int> &gateCapacities, int *solCost, Instance *inst){
    int newCost = 999999;
    /*Funcao que não recalcula a func. custo*/
    for(int i = 0; i < newGrps.size(); i++){
        for(int j = i+1; j < newGrps.size();j++){
            //Vou trocar todos os elementos do grupo I com o grupo J e checar o custo de cada trocar
            Group *grp1 = &newGrps[i];
            Group *grp2 = &newGrps[j];
            int oldCost = grp1->cost() + grp2->cost();
            //Agora vamos fazer um laço para percorrer os clientes de cada grupo
            for(int iFoo = 0; iFoo < grp1->nClients; iFoo++){
                for(int jFoo = 0; jFoo < grp2->nClients; jFoo++){
                    int clientDemand1 = inst->clientBandwidth[grp1->clients->at(iFoo)];
                    int clientDemand2 = inst->clientBandwidth[grp2->clients->at(jFoo)];
                    int gateIFoo = grp1->clientGateway[iFoo];
                    int gateJFoo = grp2->clientGateway[jFoo];
                    if( (gateCapacities[gateIFoo] + clientDemand1 - clientDemand2 < 0 ) ||
                        (gateCapacities[gateJFoo] + clientDemand2 - clientDemand1) < 0){
                        continue;//Esses clientes não podem ser trocados, isso vai estourar a capacidade dos gates
                    }
                    grp1->clientGateway[iFoo] = gateJFoo;
                    grp2->clientGateway[jFoo] = gateIFoo;
                    //newCost = calcCost(newGrps);
                    newCost = grp1->cost() + grp2->cost();
                    //Aqui precisamos recalcular a função
                    if(newCost < oldCost){
                        //*solCost = newCost;
                        gateCapacities[gateIFoo] += clientDemand1 - clientDemand2;
                        gateCapacities[gateJFoo] += clientDemand2 - clientDemand1;
                        return true;//Nova solucao :)
                    }
                    else{
                        grp1->clientGateway[iFoo] = gateIFoo;
                        grp2->clientGateway[jFoo] = gateJFoo;
                        //Volta ao estado original :]
                    }
                }
            }
        }
    }

    //Se essa otimizacao nao der certo, vamos tentar de outro jeito.
    //Vamos apenas trocar um cliente de um gateway para outro.
    //Para todos os clientes, vamos tentar todos os gateways
    for(int j = 0; j < newGrps.size(); j++){
      int oldCost = newGrps[j].cost();
      for(int k = 0; k < newGrps[j].clients->size(); k++){
        //Agora vou em todos os gateways e vou trocando os clientes
        int gateway = newGrps[j].clientGateway[k];
        for(int l = 0; l < inst->nGateways; l++){
          //Agora vamos trocando e calculando o custo. Vamos ver
          //O cliente K do grupo J vai para o gateway L
          int demand = inst->clientBandwidth[newGrps[j].clients->at(k)];
          if(demand > gateCapacities[l]) continue;
          else{
            newGrps[j].clientGateway[k] = l;
            //newCost = calcCost(newGrps);
            newCost = newGrps[j].cost();
            if (newCost < oldCost ){
              //*solCost = newCost;
              gateCapacities[l] -= demand;
              if (gateway != DUMMY) gateCapacities[gateway] += demand;
              return true;
            }else{
              newGrps[j].clientGateway[k] = gateway;
            }
          }
        }
      }
    }


    return false;
}

void disturb(vector<Group> &grps, vector<int> &gateCapacities, Instance *inst){
  //Vamos randomizar todos os clients de todos os grupos
  for(int i =0 ; i < inst->nGateways; i++) gateCapacities[i] = inst->gCapacity;
  for(int i = 0; i < grps.size();i++){
    for(int j =0; j < grps[i].nClients; j++){
      //Aqui eu tenho que aleatorizar a parada
      int seed = i + j + grps[i].nClients + time(NULL) + grps[i].clientGateway[j];
      srand(seed);
      int randomGate = rand() % inst->nGateways;
      int clientDemand = inst->clientBandwidth[grps[i].clients->at(j)];
      if (gateCapacities[randomGate] < clientDemand) grps[i].clientGateway[j] = DUMMY;
      else{
        grps[i].clientGateway[j] = randomGate;
        gateCapacities[randomGate] -= clientDemand;
      }
    }
  }
}

bool disturb2(vector<Group> &grps, vector<int> &gateCapacities, Instance *inst){
  //Nessa perturbação eu vou escolher dois clientes aleatórios e trocar os valores dos gateways dos mesmos
  int grp1 = -1, grp2 = -1;

  while(grp1 == grp2){
    grp1 = rand() % inst->nGroups;
    grp2 = rand() % inst->nGroups;
  }
  int client1, client2;
  client1 = rand() % grps[grp1].nClients;
  client2 = rand() % grps[grp2].nClients;
  int gate1, gate2, demand1, demand2;
  demand1 = inst->clientBandwidth[grps[grp1].clients->at(client1)];
  demand2 = inst->clientBandwidth[grps[grp2].clients->at(client2)];
  gate1 = grps[grp1].clientGateway[client1];
  gate2 = grps[grp2].clientGateway[client2];

  if((gateCapacities[gate1]+demand1 < demand2 ) || (gateCapacities[gate2]+ demand2 < demand1)) return false;//Solução não aceitável: viola a capacidade de um gateway
  else{
    //Troca os dois clients de gate
    gateCapacities[gate1] = gateCapacities[gate1] - demand2 + demand1;
    gateCapacities[gate2] = gateCapacities[gate2] - demand1 + demand2;
    grps[grp1].clientGateway[client1] = gate2;
    grps[grp2].clientGateway[client2] = gate1;
  }
  return true;
}

void ILS(vector<Group> &grps, vector<int> &gateCapacities, int *solCost, Instance *inst){
  //Vamos lá, preciso definir primeiro um KMAX
  int kmax = 100;
  vector<Group> bestGrp(grps);
  vector<int> newGateCapacities(gateCapacities);
  int newCost;
  std::ofstream out;
  out.open("result.out");
  //Agora vamos lá hein.
  srand(time(NULL));
  cout << "Disturbing solutions now" << endl;
  for(int i = 0; i < kmax; i++){
    vector<Group> disturbedGrp(bestGrp);
    //cout << "Disturbance " << i+1 << endl;
    //while(!disturb2(disturbedGrp, newGateCapacities, inst));
    disturb(disturbedGrp, newGateCapacities, inst);
    //Agora vamos optimizar >]
    while(localSearchCostOPT(disturbedGrp, newGateCapacities, &newCost, inst));
    newCost = calcCost(disturbedGrp);
    if(newCost < *solCost) {
      cout << "New solution found. Cost: " << newCost << endl << endl;
      bestGrp.swap(disturbedGrp);
      *solCost = newCost;
    }
  }


  //Falta imprimir a solução

  out << endl << endl << "Solution after ILS" << endl;

  for (int k = 0; k < inst->nGroups; k++) {
      out << "Group " << k << endl;

      for (int l = 0; l < bestGrp[k].nClients; l++) {
          out << (*(bestGrp[k].clients))[l] << " - " <<
              bestGrp[k].clientGateway[l] << endl;
      }
  }

  out << "ILS procedure ended" << endl;
  out << "Best solution with cost " << *solCost << endl;
  out << "Max function: " << calcCostMax(bestGrp);
  cout << "Best solution with cost " << *solCost << endl;
}

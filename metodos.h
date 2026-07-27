#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>
#include "Data.h"

typedef struct Solucao
{
    vector<int> sequence;
    double Valor_obj;
}  Solution;

#include "auxiliar.h"

using namespace std;

typedef struct insert_info
{
    int no_inserido;
    int aresta_removida;
    double custo;
} Insertion;



void exibir_solution(Solution s){
    for(int i = 0; i < s.sequence.size(); i++){
        std:: cout << s.sequence[i] << " -> ";
    }

    std:: cout << std:: endl;

    std:: cout << "Custo Total: " << s.Valor_obj;
}

void calcular_obj(Solution *s, Data data){
    s->Valor_obj = 0;
    for(int i =1; i < s->sequence.size() - 1; i++){
        int j = s->sequence[i];
        int k = s->sequence[i+1];

        s->Valor_obj += data.getDistance(j,k);
    }
}


Solution ILS(int Max_iter, int max_Iter_Ils, Data data){
    Solution best_of_all;
    best_of_all.Valor_obj = INFINITY;
    
    for (int i = 0; i < Max_iter; i++){
        Solution s = construcao(data);
        Solution best = s;

        int Iter_Ils = 0;

        while( Iter_Ils < best.Valor_obj){
            busca_local(&s, data);
            if(s.Valor_obj < best.Valor_obj){
                best = s;
                Iter_Ils = 0;
            }

            //s = Pertubacao(best);
            Iter_Ils++;
        }

        if (best.Valor_obj < best_of_all.Valor_obj)
            best_of_all.Valor_obj = best.Valor_obj;
    }

    return best_of_all;

    exibir_solution(best_of_all);
}

std:: vector<Insertion> calcular_custo_insert(Solution& s, std:: vector<int>& CL, Data& data){
    std:: vector<Insertion> custo_insert = std:: vector<Insertion>((s.sequence.size() - 1) * CL.size());

    for(int a = 1; a < s.sequence.size() - 1; a++){

        int i = s.sequence[a];
        int j = s.sequence[a + 1];

        for (auto k : CL){
            Insertion ins;

            ins.custo =  data.getDistance(i,k) + data.getDistance(j,k) - data.getDistance(i,j);
            ins.no_inserido = k;
            ins.aresta_removida = a;
            custo_insert.push_back(ins);
        }
    }
    return custo_insert;
}

void insert_random_v_em_s(Solution *s,vector<int> CL, Data& data){
    int limit = data.getDimension();
    std:: random_device rd;
    std:: mt19937 gen(rd());
    std:: uniform_int_distribution<> dist(1, limit);

    while (s->sequence.size() < 3){
        int no = dist(gen);
        if(find(s->sequence.begin(), s->sequence.end(), no) == s->sequence.end()){
            s->sequence.push_back(no);
            CL.erase(remove(CL.begin(), CL.end(), no), CL.end());
        }
    }
    s->sequence.push_back(s->sequence[0]);
}

void busca_local(Solution *s, Data data){
    std:: vector <int> NL = {1,2,3,4,5};

    bool improved = false;

    while(NL.empty() == false){
        int n = rand() % NL.size();
        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(&s, data);
            break;
        
        default:
            break;
        }
    }
}

Solution construcao(Data data){

    Solution s;
    std:: vector<int> CL;

    for (int i = 1; i <= data.getDimension(); i++){
        CL.push_back(i);
    }

    insert_random_v_em_s(&s,CL, data);


    while(!CL.empty()){
        std:: vector<Insertion> custo_insert = calcular_custo_insert(s, CL, data);
        
        sort(custo_insert.begin(), custo_insert.end());

        double alpha = (double) rand() / RAND_MAX;

        int selecionado = rand() % ((int) ceil(alpha*custo_insert.size()));
        int custo_a_ser_adicionado = custo_insert[selecionado].no_inserido;
        Insertion escolhida = custo_insert[selecionado];

        s.sequence.insert(s.sequence.begin() + escolhida.aresta_removida + 1, escolhida.no_inserido);

        CL.erase(std::remove(CL.begin(), CL.end(), escolhida.no_inserido), CL.end());

    }

    calcular_obj(&s, data);
    return s;
}
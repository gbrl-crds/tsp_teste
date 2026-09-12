#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <random>
#include <cstdlib>
#include <chrono>
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



void exibir_solution(double media_custo, double media_tempo){

    std:: cout << std:: endl;

    std:: cout << "Custo Total Medio: " << media_custo << endl;

    std:: cout << "Tempo Medio do Programa: " << media_tempo << endl;
}

void calcular_obj(Solution *s, Data& data){
    s->Valor_obj = 0;

    int cont = s->sequence.size();

    for(int i =0; i < (cont - 1); i++){
        int j = s->sequence[i];
        int k = s->sequence[i+1];

        s->Valor_obj += data.getDistance(j,k);
    }
}




std:: vector<Insertion> calcular_custo_insert(Solution& s, std:: vector<int>& CL, Data& data){

    size_t tam;

    if(!s.sequence.empty()){
        tam = (s.sequence.size() - 1) * CL.size();
    }
    std:: vector<Insertion> custo_insert;

    int cont = s.sequence.size();

    for(int a = 0; a < (cont - 1); a++){

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

void insert_random_v_em_s(Solution *s,vector<int>& CL, Data& data){
    int limit = data.getDimension();
    std:: random_device rd;
    std:: mt19937 gen(rd());
    std:: uniform_int_distribution<> dist(1, limit);

    int cont = s->sequence.size();

    while (cont < 3){
        int no = dist(gen);
        if(find(s->sequence.begin(), s->sequence.end(), no) == s->sequence.end()){
            s->sequence.push_back(no);
            CL.erase(remove(CL.begin(), CL.end(), no), CL.end());
            cont++;
        }
        
    }
    s->sequence.push_back(s->sequence[0]);
}

void busca_local(Solution *s, Data data){
    std:: vector <int> NL = {1, 2, 3, 4, 5};


    while(NL.empty() == false){
        int n = rand() % NL.size();
        int vizi = NL[n];
        bool improved = false;

        switch (NL[n])
        {
        case 1:
            improved = bestImprovementSwap(*s, data);
            break;
        
        case 2: 
            improved = bestImprovement2Opt(*s, data);
            break;

        case 3:
            improved = bestImprovementOrOpt(*s, data, 1); // Reinsertion
            break;
        
        case 4:
            improved = bestImprovementOrOpt(*s, data, 2); //or-opt-2
            break;

        case 5: improved = bestImprovementOrOpt(*s, data, 3);//or-opt-3

        default:
            break;
        }

        if(improved){
            NL = {1,2,3,4,5};
        }else{ 
            NL.erase(NL.begin());
        }
        
    }
}

bool comp(Insertion& custoa, Insertion& custob){

    return custoa.custo < custob.custo;

}

Solution construcao(Data data){

    Solution s;
    std:: vector<int> CL;

    int cont = data.getDimension();

    for (int i = 1; i <= cont; i++){
        CL.push_back(i);
    }
    

    insert_random_v_em_s(&s,CL, data);

    while(!CL.empty()){

        
        std:: vector<Insertion> custo_insert = calcular_custo_insert(s, CL, data);
        
        
        sort(custo_insert.begin(), custo_insert.end(), comp);

        double alpha = (double) rand() / RAND_MAX;

        int tam = ((int) ceil(alpha*custo_insert.size()));

        if (tam < 1) tam = 1;

        int selecionado = rand() % tam;

        int custo_a_ser_adicionado = custo_insert[selecionado].no_inserido;
        Insertion escolhida = custo_insert[selecionado];

        s.sequence.insert(s.sequence.begin() + escolhida.aresta_removida + 1, escolhida.no_inserido);

        CL.erase(std::remove(CL.begin(), CL.end(), escolhida.no_inserido), CL.end());

        auto it = std::find(CL.begin(), CL.end(), escolhida.no_inserido);
        if (it != CL.end()) {
            CL.erase(it);
        }

    }

    calcular_obj(&s, data);
    return s;
}

void Pertubacao(Solution* s, Data& data){
    int n = s->sequence.size();

    int lim_sup = (data.getDimension()/10);
    int lim_inf = 2;

    double alpha1 = (double) rand() / RAND_MAX;
    double alpha2 = (double) rand() / RAND_MAX;

    int tam1 = lim_inf + (int)(alpha1 * (lim_sup - lim_inf + 1));
    int tam2 = lim_inf + (int)(alpha2 * (lim_sup - lim_inf + 1));

    tam1 = std::clamp(tam1, lim_inf, lim_sup);
    tam2 = std::clamp(tam2, lim_inf, lim_sup);

    int max_pos1 = n - 1 - tam1 - tam2;
    if (max_pos1 <= 1) return;

    int pos1 = 1 + (rand() % max_pos1);
    int pos2 = pos1 + tam1 + (rand() % (n - 1 - (pos1 + tam1) - tam2 + 1));

    std::vector<int> seg1(s->sequence.begin() + pos1, s->sequence.begin() + pos1 + tam1);
    std::vector<int> seg2(s->sequence.begin() + pos2, s->sequence.begin() + pos2 + tam2);

    s->sequence.erase(s->sequence.begin() + pos2, s->sequence.begin() + pos2 + tam2);
    s->sequence.insert(s->sequence.begin() + pos2, seg1.begin(), seg1.end());

    s->sequence.erase(s->sequence.begin() + pos1, s->sequence.begin() + pos1 + tam1);
    s->sequence.insert(s->sequence.begin() + pos1, seg2.begin(), seg2.end());

    

}

Solution ILS(int Max_iter, int max_Iter_Ils, Data data){


    Solution best_of_all;
    Solution best;
    best_of_all.Valor_obj = best.Valor_obj = INFINITY;
    
    for (int i = 0; i < Max_iter; i++){
        Solution s = construcao(data);
        busca_local(&s, data);
        
        best = s;
        int Iter_Ils = 0;

        while( Iter_Ils < max_Iter_Ils){
            Solution s_temp = best;

            Pertubacao(&s_temp, data);
            calcular_obj(&s_temp, data);
            busca_local(&s_temp, data);

            if(s_temp.Valor_obj < best.Valor_obj){
                best = s_temp;
                Iter_Ils = 0;
            }else{
                Iter_Ils++;
            }
            
        }

        if (best.Valor_obj < best_of_all.Valor_obj){
            best_of_all = best;
        }
            
    }
    return best_of_all;
}
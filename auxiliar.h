#pragma once
#include <iostream>
#include <vector>
#include <random>
#include "metodos.h"

bool bestImprovementSwap(Solution& s, Data& data){
    double best_delta = 0;
    int best_i = -1, best_j =-1;


    for(int i = 1; i < s.sequence.size() - 2; i++){
        int vi = s.sequence[i];
        int vi_next = s.sequence[i+1];
        int vi_prev = s.sequence[i-1];

        for (int j = i+1; j < s.sequence.size() -1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];
            int vj_prev = s.sequence[j-1];
            
            double delta;

            if (j == i+1){
                delta = ((-data.getDistance(vi_prev, vi)) + data.getDistance(vi_prev, vj)
                         - data.getDistance(vj, vj_next) + data.getDistance(vi, vj_next));
            } else{
                delta = (-data.getDistance(vi_prev, vi) + data.getDistance(vi_prev, vj)
                         -data.getDistance(vi, vi_next) + data.getDistance(vj, vi_next)
                         -data.getDistance(vj, vj) + data.getDistance(vj_prev, vi) 
                         -data.getDistance(vj, vj_next) + data.getDistance(vi, vj_next));

            }

            
            if (delta < best_delta ){
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }   
    }


    if (best_delta < 0){
        std:: swap(s.sequence[best_i], s.sequence[best_j]);
        s.Valor_obj += best_delta;

        return true;
    }
    return false;
}

bool bestImprovement2Opt(Solution& s, Data& data){
    double best_delta = 0;
    int best_i, best_j;
    for(int i = 1; i < s.sequence.size() -1; i++){
        int vi = s.sequence[i];
        int vi_prev = s.sequence[i-1];

        for(int j = i+1; j < s.sequence.size() -1; j++){
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            double delta =  (-data.getDistance(vi_prev, vi)
                             +data.getDistance(vi_prev, vj)
                             -data.getDistance(vj, vj_next)
                             +data.getDistance(vi, vj_next));
            
            if (delta < best_delta){
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_delta < 0){

            std::reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);

            s.Valor_obj += best_delta;

            return true;
    }
    
    return false;
}

bool bestImprovementOrOpt(Solution& s, Data& data, int esco){
    int best_i, best_j;
    double best_delta =0;
    double delta =0;
    int cont = s.sequence.size();

    for (int i = 1; i < (cont -esco); i++){
        int vi = s.sequence[i];
        int vi_prev = s.sequence[i-1];

        for (int j = 0; j < (cont - 1); j++){

            if (j >= i-1 && j < i + esco) continue;
            int vj = s.sequence[j];
            int vj_next = s.sequence[j+1];

            switch(esco)
            {
            case 1:{
                int vi_next = s.sequence[i + 1];
                delta = (-data.getDistance(vj, vj_next)
                         +data.getDistance(vi, vj_next)
                         +data.getDistance(vi,vj)
                         -data.getDistance(vi_prev, vi)
                         -data.getDistance(vi,vi_next)
                         +data.getDistance(vi_prev, vi_next));

                break;}

            case 2:{
                int vi_next = s.sequence[i + 1];
                int vi_next_2 = s.sequence[i + 2];
                delta = (data.getDistance(vj,vi)) + 
                            data.getDistance(vi_next, vj_next) -
                            data.getDistance(vj, vj_next) -
                            data.getDistance(vi_prev, vi) -
                            data.getDistance(vi_next, vi_next_2) +
                            data.getDistance(vi_prev, vi_next_2);
                break;}

            case 3:{
                int vi_next_2 = s.sequence[i+2];
                int vi_next_3 = s.sequence[i+3];
                delta = (data.getDistance(vj,vi)) + 
                            data.getDistance(vi_next_2, vj_next) -
                            data.getDistance(vj, vj_next) -
                            data.getDistance(vi_prev, vi) -
                            data.getDistance(vi_next_2, vi_next_3) +
                            data.getDistance(vi_prev, vi_next_3);
                break;}
            }   
            
            
            if (delta < best_delta){
            best_delta = delta;
            best_i = i;
            best_j = j;
            }
        }
    }

    if (best_delta < 0){
        std:: vector <int> bloco(s.sequence.begin() + best_i, s.sequence.begin() + best_i + esco);

        s.sequence.erase(s.sequence.begin() + best_i, s.sequence.begin() + best_i + esco);

        if (best_i < best_j) {
            s.sequence.insert(s.sequence.begin() + (best_j - esco + 1), bloco.begin(), bloco.end());
        } else {
            s.sequence.insert(s.sequence.begin() + best_j + 1, bloco.begin(), bloco.end());
        }
        s.Valor_obj += best_delta; 
        return true;
    }   


    return false;
    
}
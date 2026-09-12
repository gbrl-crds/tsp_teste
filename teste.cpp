#include "Data.h"
#include "metodos.h"
#include <iostream>
#include <chrono>

using namespace std;


int main(int argc, char** argv) {

    double soma_tempo = 0;
    double soma_custo = 0;


    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;

    cout << "Exemplo de Solucao s = " << endl;
    
    
    for (int i = 0; i < 10; i++){
        auto inicio = std::chrono::high_resolution_clock::now();


        Solution sol = ILS(50, 140, data);

        auto fim = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> tempo = fim - inicio;

        soma_tempo += tempo.count();

        soma_custo += sol.Valor_obj;

    }

    double media_tempo = (soma_tempo)/10;
    double media_custo = (soma_custo)/10;

    exibir_solution(media_custo,media_tempo);

    

    return 0;
}
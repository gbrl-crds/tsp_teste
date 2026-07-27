#include "Data.h"
#include "metodos.h"
#include <iostream>

using namespace std;


int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();

    int max_iter = std::stoi(argv[2]);     
    int max_iter_ils = std::stoi(argv[3]);

    cout << "Exemplo de Solucao s = ";
    
    ILS(max_iter, max_iter_ils, data);

    return 0;
}
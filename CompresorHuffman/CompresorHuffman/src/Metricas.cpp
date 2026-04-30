#include "Metricas.h"

using namespace std;

int Metricas::calcularBitsOriginalASCII(const string& texto) {
    // Base de comparacion: cada caracter ocupa 8 bits en ASCII fijo.
    return static_cast<int>(texto.size()) * 8;
}

int Metricas::calcularBitsHuffman(const string& bits) {
    // Cada posicion del string representa un bit codificado.
    return static_cast<int>(bits.size());
}

double Metricas::calcularPorcentajeCompresion(int bitsOriginal, int bitsComprimido) {
    if (bitsOriginal == 0) {
        return 0.0;
    }

    // Valor > 0 indica reduccion; valor < 0 indica expansion.
    return (1.0 - (static_cast<double>(bitsComprimido) / static_cast<double>(bitsOriginal))) * 100.0;
}

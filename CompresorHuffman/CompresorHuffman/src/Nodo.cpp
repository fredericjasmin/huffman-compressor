#include "Nodo.h"

using namespace std;

// Constructor para nodos hoja (simbolo real del texto).
Nodo::Nodo(char simbolo, int frecuencia)
    : simbolo(simbolo), frecuencia(frecuencia), izquierdo(nullptr), derecho(nullptr) {
}

// Constructor para nodos internos (combinacion de dos subarboles).
Nodo::Nodo(int frecuencia, Nodo* izquierdo, Nodo* derecho)
    : simbolo('\0'), frecuencia(frecuencia), izquierdo(izquierdo), derecho(derecho) {
}

bool Nodo::esHoja() const {
    // En Huffman, solo las hojas representan simbolos del texto original.
    return izquierdo == nullptr && derecho == nullptr;
}

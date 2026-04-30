#pragma once

using namespace std;

// Nodo del arbol de Huffman.
class Nodo {
public:
    // simbolo solo se usa en hojas; en nodos internos queda en '\0'.
    char simbolo;
    // frecuencia acumulada del subarbol.
    int frecuencia;
    // punteros a hijos para representar el arbol binario.
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(char simbolo, int frecuencia);
    Nodo(int frecuencia, Nodo* izquierdo, Nodo* derecho);

    // Un nodo hoja no tiene hijos.
    bool esHoja() const;
};

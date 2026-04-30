#pragma once

#include <vector>
#include "Nodo.h"

using namespace std;

// Cola de prioridad minima implementada manualmente para construir el arbol de Huffman.
class MinHeap {
private:
    // Arreglo dinamico con representacion de heap binario completo.
    vector<Nodo*> datos;

    // Reacomoda un elemento hacia arriba para restaurar heap minimo.
    void subir(int indice);
    // Reacomoda un elemento hacia abajo para restaurar heap minimo.
    void bajar(int indice);
    void intercambiar(int i, int j);

public:
    // Inserta un nodo manteniendo la propiedad de prioridad minima.
    void insertar(Nodo* nodo);
    // Devuelve el nodo de menor frecuencia.
    Nodo* extraerMinimo();
    // Cantidad de nodos actualmente en el heap.
    int tamano() const;
    // Indica si el heap no tiene elementos.
    bool vacio() const;
};

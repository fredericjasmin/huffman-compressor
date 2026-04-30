#include "MinHeap.h"

using namespace std;

void MinHeap::intercambiar(int i, int j) {
    // Intercambio directo para mantener operacion O(1).
    Nodo* temp = datos[i];
    datos[i] = datos[j];
    datos[j] = temp;
}

void MinHeap::subir(int indice) {
    while (indice > 0) {
        int padre = (indice - 1) / 2;
        // Mantiene la propiedad de heap minimo: padre <= hijos.
        if (datos[indice]->frecuencia < datos[padre]->frecuencia) {
            intercambiar(indice, padre);
            indice = padre;
        }
        else {
            break;
        }
    }
}

void MinHeap::bajar(int indice) {
    int n = static_cast<int>(datos.size());

    while (true) {
        // Posiciones de hijos en arreglo para heap binario.
        int izquierdo = 2 * indice + 1;
        int derecho = 2 * indice + 2;
        int menor = indice;

        if (izquierdo < n && datos[izquierdo]->frecuencia < datos[menor]->frecuencia) {
            menor = izquierdo;
        }

        if (derecho < n && datos[derecho]->frecuencia < datos[menor]->frecuencia) {
            menor = derecho;
        }

        if (menor != indice) {
            intercambiar(indice, menor);
            indice = menor;
        }
        else {
            break;
        }
    }
}

void MinHeap::insertar(Nodo* nodo) {
    datos.push_back(nodo);
    // El nuevo nodo entra al final y sube segun su prioridad.
    subir(static_cast<int>(datos.size()) - 1);
}

Nodo* MinHeap::extraerMinimo() {
    if (datos.empty()) {
        // No hay elemento para devolver.
        return nullptr;
    }

    Nodo* minimo = datos[0];
    // Se mueve el ultimo elemento a la raiz y luego se reacomoda hacia abajo.
    datos[0] = datos.back();
    datos.pop_back();

    if (!datos.empty()) {
        bajar(0);
    }

    return minimo;
}

int MinHeap::tamano() const {
    // Se devuelve como int por simplicidad en las comparaciones del flujo.
    return static_cast<int>(datos.size());
}

bool MinHeap::vacio() const {
    return datos.empty();
}

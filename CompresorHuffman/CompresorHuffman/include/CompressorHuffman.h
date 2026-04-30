#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "Nodo.h"
#include "Metricas.h"

using namespace std;

// Encapsula todo el flujo de Huffman: analisis, codificacion, decodificacion y reporte.
class CompressorHuffman {
private:
    // Estado base de trabajo.
    string textoOriginal;
    unordered_map<char, int> frecuencias;
    unordered_map<char, string> codigos;

    // Estructuras derivadas del proceso.
    Nodo* raiz;
    string textoComprimido;
    string textoDescomprimido;

    MetricasResultado metricas;

    // Limpia arbol y estructuras para iniciar un nuevo ciclo de compresion.
    void limpiarEstructuras();
    void liberarArbol(Nodo* nodo);
    // DFS para construir la tabla simbolo -> codigo.
    void generarCodigosDFS(Nodo* nodo, const string& codigoActual);

public:
    CompressorHuffman();
    ~CompressorHuffman();

    bool ingresarTexto(const string& texto);
    bool hayTexto() const;

    bool calcularFrecuencias();
    bool construirArbol();
    bool generarCodigos();
    bool comprimir();
    bool descomprimir();
    bool verificarIntegridad() const;

    // Verificacion de consistencia: detecta si algun codigo es prefijo de otro.
    bool validarPrefijoLibre() const;

    void mostrarFrecuencias() const;
    void mostrarCodigos() const;
    void mostrarTextoComprimido() const;
    void mostrarTextoDescomprimido() const;
    void mostrarMetricasYComparacion() const;

    const string& obtenerTextoOriginal() const;
    const string& obtenerTextoComprimido() const;
    const string& obtenerTextoDescomprimido() const;
};

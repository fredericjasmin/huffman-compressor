#pragma once

#include <string>

using namespace std;

// Resultado acumulado para mostrar el analisis del proceso de compresion.
struct MetricasResultado {
    // Tamaño base con codificacion fija (ASCII 8 bits por caracter).
    int bitsOriginal;
    // Tamaño final del texto codificado con Huffman.
    int bitsHuffman;
    // Porcentaje de reduccion respecto al tamaño original.
    double porcentajeCompresion;
    // Tiempos de ejecucion en milisegundos.
    double tiempoCompresionMs;
    double tiempoDescompresionMs;
};

class Metricas {
public:
    // Convierte cantidad de caracteres a bits usando 8 bits por caracter.
    static int calcularBitsOriginalASCII(const string& texto);
    // El string comprimido ya representa bits en formato '0' y '1'.
    static int calcularBitsHuffman(const string& bits);
    // Formula: (1 - comprimido/original) * 100.
    static double calcularPorcentajeCompresion(int bitsOriginal, int bitsComprimido);
};

#include "CompressorHuffman.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include "MinHeap.h"

using namespace std;

CompressorHuffman::CompressorHuffman() : raiz(nullptr) {
    // Estado inicial sin resultados calculados.
    metricas = { 0, 0, 0.0, 0.0, 0.0 };
}

CompressorHuffman::~CompressorHuffman() {
    limpiarEstructuras();
}

void CompressorHuffman::limpiarEstructuras() {
    // Siempre liberar el arbol antes de resetear punteros para evitar fugas.
    liberarArbol(raiz);
    raiz = nullptr;

    frecuencias.clear();
    codigos.clear();
    textoComprimido.clear();
    textoDescomprimido.clear();
    metricas = { 0, 0, 0.0, 0.0, 0.0 };
}

void CompressorHuffman::liberarArbol(Nodo* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Postorden para liberar primero hijos y luego el nodo actual.
    liberarArbol(nodo->izquierdo);
    liberarArbol(nodo->derecho);
    delete nodo;
}

bool CompressorHuffman::ingresarTexto(const string& texto) {
    if (texto.empty()) {
        return false;
    }

    // Cada nuevo texto invalida estructuras previas.
    limpiarEstructuras();
    textoOriginal = texto;
    return true;
}

bool CompressorHuffman::hayTexto() const {
    return !textoOriginal.empty();
}

bool CompressorHuffman::calcularFrecuencias() {
    if (!hayTexto()) {
        return false;
    }

    frecuencias.clear();
    // Recuento lineal: cada caracter incrementa su contador.
    for (char c : textoOriginal) {
        frecuencias[c]++;
    }

    return true;
}

bool CompressorHuffman::construirArbol() {
    if (frecuencias.empty()) {
        return false;
    }

    liberarArbol(raiz);
    raiz = nullptr;

    MinHeap heap;
    for (auto& par : frecuencias) {
        // Se crea un nodo hoja por simbolo distinto.
        heap.insertar(new Nodo(par.first, par.second));
    }

    if (heap.tamano() == 1) {
        // Caso limite: si solo existe un simbolo, se crea una raiz padre para conservar
        // la logica de recorrido y permitir codificar con al menos 1 bit.
        Nodo* unico = heap.extraerMinimo();
        raiz = new Nodo(unico->frecuencia, unico, nullptr);
        return true;
    }

    while (heap.tamano() > 1) {
        // Regla central de Huffman: combinar siempre las dos menores frecuencias.
        Nodo* a = heap.extraerMinimo();
        Nodo* b = heap.extraerMinimo();

        Nodo* combinado = new Nodo(a->frecuencia + b->frecuencia, a, b);
        heap.insertar(combinado);
    }

    raiz = heap.extraerMinimo();
    return raiz != nullptr;
}

void CompressorHuffman::generarCodigosDFS(Nodo* nodo, const string& codigoActual) {
    if (nodo == nullptr) {
        return;
    }

    if (nodo->esHoja()) {
        if (codigoActual.empty()) {
            // Si el arbol tiene un unico simbolo, su codigo se define como "0".
            codigos[nodo->simbolo] = "0";
        }
        else {
            codigos[nodo->simbolo] = codigoActual;
        }
        return;
    }

    generarCodigosDFS(nodo->izquierdo, codigoActual + "0");
    generarCodigosDFS(nodo->derecho, codigoActual + "1");
}

bool CompressorHuffman::generarCodigos() {
    if (raiz == nullptr) {
        return false;
    }

    codigos.clear();
    // Al iniciar en raiz con codigo vacio, el DFS asigna 0/1 por rama.
    generarCodigosDFS(raiz, "");
    return !codigos.empty();
}

bool CompressorHuffman::comprimir() {
    if (codigos.empty() || !hayTexto()) {
        return false;
    }

    clock_t inicio = clock();

    textoComprimido.clear();
    // Sustituye cada simbolo por su codigo variable.
    for (char c : textoOriginal) {
        textoComprimido += codigos[c];
    }

    clock_t fin = clock();
    double duracionMs = (static_cast<double>(fin - inicio) * 1000.0) / CLOCKS_PER_SEC;

    metricas.bitsOriginal = Metricas::calcularBitsOriginalASCII(textoOriginal);
    metricas.bitsHuffman = Metricas::calcularBitsHuffman(textoComprimido);
    // El porcentaje es positivo cuando Huffman usa menos bits que ASCII fijo.
    metricas.porcentajeCompresion = Metricas::calcularPorcentajeCompresion(metricas.bitsOriginal, metricas.bitsHuffman);
    metricas.tiempoCompresionMs = duracionMs;

    return true;
}

bool CompressorHuffman::descomprimir() {
    if (raiz == nullptr || textoComprimido.empty()) {
        return false;
    }

    clock_t inicio = clock();

    textoDescomprimido.clear();
    // Se usa estructura lineal para acumular salida antes de formar el string final.
    vector<char> salida;
    Nodo* actual = raiz;

    for (char bit : textoComprimido) {
        if (bit == '0') {
            actual = actual->izquierdo;
        }
        else if (bit == '1') {
            actual = actual->derecho;
        }
        else {
            // Si aparece un simbolo distinto de 0/1 el stream es invalido.
            return false;
        }

        if (actual == nullptr) {
            // Camino inexistente en el arbol: datos comprimidos inconsistentes.
            return false;
        }

        if (actual->esHoja()) {
            salida.push_back(actual->simbolo);
            // Reinicia en la raiz para decodificar el siguiente simbolo.
            actual = raiz;
        }
    }

    for (char c : salida) {
        textoDescomprimido += c;
    }

    clock_t fin = clock();
    double duracionMs = (static_cast<double>(fin - inicio) * 1000.0) / CLOCKS_PER_SEC;
    metricas.tiempoDescompresionMs = duracionMs;

    return true;
}

bool CompressorHuffman::verificarIntegridad() const {
    // Valida que no se altero informacion durante compresion/descompresion.
    return textoOriginal == textoDescomprimido;
}

bool CompressorHuffman::validarPrefijoLibre() const {
    vector<string> lista;
    // Se toma solo la lista de codigos para comparar prefijos entre pares.
    for (auto& par : codigos) {
        lista.push_back(par.second);
    }

    for (size_t i = 0; i < lista.size(); i++) {
        for (size_t j = 0; j < lista.size(); j++) {
            if (i == j) {
                continue;
            }

            const string& a = lista[i];
            const string& b = lista[j];

            if (a.size() <= b.size()) {
                // Si un codigo es prefijo de otro, la tabla no es valida.
                if (b.substr(0, a.size()) == a) {
                    return false;
                }
            }
        }
    }

    return true;
}

void CompressorHuffman::mostrarFrecuencias() const {
    if (frecuencias.empty()) {
        cout << "No hay frecuencias calculadas.\n";
        return;
    }

    cout << "Frecuencias:\n";
    // Se muestra [espacio] para que el reporte no quede ambiguo visualmente.
    for (auto& par : frecuencias) {
        if (par.first == ' ') {
            cout << "[espacio]";
        }
        else {
            cout << par.first;
        }
        cout << ": " << par.second << "\n";
    }
}

void CompressorHuffman::mostrarCodigos() const {
    if (codigos.empty()) {
        cout << "No hay codigos generados.\n";
        return;
    }

    cout << "Codigos Huffman:\n";
    // Los codigos pueden variar por empates de frecuencia y siguen siendo correctos.
    for (auto& par : codigos) {
        if (par.first == ' ') {
            cout << "[espacio]";
        }
        else {
            cout << par.first;
        }
        cout << ": " << par.second << "\n";
    }
}

void CompressorHuffman::mostrarTextoComprimido() const {
    if (textoComprimido.empty()) {
        cout << "No hay texto comprimido.\n";
        return;
    }

    // Se imprime como cadena de 0/1 para facilitar verificacion en consola.
    cout << "Texto comprimido (bits):\n" << textoComprimido << "\n";
}

void CompressorHuffman::mostrarTextoDescomprimido() const {
    if (textoDescomprimido.empty()) {
        cout << "No hay texto descomprimido.\n";
        return;
    }

    cout << "Texto descomprimido:\n" << textoDescomprimido << "\n";
}

void CompressorHuffman::mostrarMetricasYComparacion() const {
    if (metricas.bitsOriginal == 0) {
        cout << "No hay metricas. Primero comprima un texto.\n";
        return;
    }

    cout << fixed << setprecision(2);
    // Mismo formato decimal en todo el reporte para comparacion consistente.
    cout << "Metricas:\n";
    cout << "Tamano original (ASCII fijo): " << metricas.bitsOriginal << " bits\n";
    cout << "Tamano comprimido (Huffman): " << metricas.bitsHuffman << " bits\n";
    cout << "Compresion: " << metricas.porcentajeCompresion << "%\n";
    cout << "Tiempo de compresion: " << metricas.tiempoCompresionMs << " ms\n";
    cout << "Tiempo de descompresion: " << metricas.tiempoDescompresionMs << " ms\n";

    cout << "\nComparacion de enfoques:\n";
    cout << "Enfoque A: Codificacion fija ASCII de 8 bits por caracter.\n";
    cout << "Enfoque B: Codificacion variable Huffman.\n";

    if (metricas.bitsHuffman < metricas.bitsOriginal) {
        cout << "Resultado: Huffman reduce el tamano frente a ASCII fijo.\n";
    }
    else if (metricas.bitsHuffman == metricas.bitsOriginal) {
        cout << "Resultado: Huffman no mejora el tamano en este texto.\n";
    }
    else {
        cout << "Resultado: Huffman aumenta el tamano en este caso.\n";
    }
}

const string& CompressorHuffman::obtenerTextoOriginal() const {
    return textoOriginal;
}

const string& CompressorHuffman::obtenerTextoComprimido() const {
    return textoComprimido;
}

const string& CompressorHuffman::obtenerTextoDescomprimido() const {
    return textoDescomprimido;
}

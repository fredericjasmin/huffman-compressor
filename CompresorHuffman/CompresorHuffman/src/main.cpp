#include <iostream>
#include <string>
#include "CompressorHuffman.h"

using namespace std;

// Palabra precargada por defecto
const string DEFAULT_TEXT = "En estructuras de datos usamos arboles y colas para resolver problemas reales";

void limpiarPantalla() {
    // Limpia la pantalla en Windows
    system("cls");
}

void mostrarMenu() {
    // Menu principal de operaciones del sistema.
    cout << "\n=== Compresor Huffman ===\n";
    cout << "1. Ingresar texto\n";
    cout << "2. Calcular frecuencias\n";
    cout << "3. Construir arbol Huffman\n";
    cout << "4. Mostrar codigos\n";
    cout << "5. Comprimir texto\n";
    cout << "6. Descomprimir texto\n";
    cout << "7. Mostrar metricas y comparacion\n";
    cout << "8. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    CompressorHuffman sistema;
    int opcion = 0;

    // Precarga automática de la palabra por defecto
    sistema.ingresarTexto(DEFAULT_TEXT);
    cout << "Texto precargado: \"" << DEFAULT_TEXT << "\"\n";
    cout << "Puedes cambiarlo con la opcion 1 si lo deseas.\n";

    do {
        mostrarMenu();

        if (!(cin >> opcion)) {
            // Recupera el flujo cuando se ingresa texto en lugar de numero.
            cout << "Entrada invalida.\n";
            cin.clear();
            // Limpia todo lo escrito en esa linea para que no rompa la siguiente lectura.
            cin.ignore(10000, '\n');
            continue;
        }

        // Limpia el salto de linea pendiente antes de leer de nuevo.
        cin.ignore(10000, '\n');
        cout << "\n";

        if (opcion == 1) {
            string texto;
            cout << "Ingrese el texto: ";
            getline(cin, texto);

            if (!sistema.ingresarTexto(texto)) {
                cout << "Error: el texto no puede ser vacio.\n";
                continue;
            }

            cout << "Texto cargado correctamente.\n";
        }
        else if (opcion == 2) {
            // Permite ver distribucion de simbolos antes de construir el arbol.
            if (!sistema.calcularFrecuencias()) {
                cout << "Primero ingrese un texto.\n";
                continue;
            }
            sistema.mostrarFrecuencias();
        }
        else if (opcion == 3) {
            // Esta opcion fuerza el flujo completo para garantizar que codigos exista.
            if (!sistema.calcularFrecuencias()) {
                cout << "Primero ingrese un texto.\n";
                continue;
            }

            if (!sistema.construirArbol()) {
                cout << "No se pudo construir el arbol.\n";
                continue;
            }

            if (!sistema.generarCodigos()) {
                cout << "No se pudieron generar los codigos.\n";
                continue;
            }

            cout << "Arbol construido y codigos generados.\n";
            cout << (sistema.validarPrefijoLibre() ? "Tabla prefijo-libre valida.\n" : "Error de prefijo-libre.\n");
        }
        else if (opcion == 4) {
            // Muestra la tabla simbolo -> codigo generada por DFS.
            sistema.mostrarCodigos();
        }
        else if (opcion == 5) {
            if (!sistema.hayTexto()) {
                cout << "Primero ingrese un texto.\n";
                continue;
            }

            // Recalcula estructuras para evitar usar estado viejo entre corridas.
            if (!sistema.calcularFrecuencias() || !sistema.construirArbol() || !sistema.generarCodigos()) {
                cout << "No fue posible preparar la compresion.\n";
                continue;
            }

            if (!sistema.comprimir()) {
                cout << "No se pudo comprimir.\n";
                continue;
            }

            sistema.mostrarTextoComprimido();
        }
        else if (opcion == 6) {
            // Solo tiene sentido despues de comprimir porque usa el stream de bits actual.
            if (!sistema.descomprimir()) {
                cout << "No se pudo descomprimir. Comprima primero.\n";
                continue;
            }

            sistema.mostrarTextoDescomprimido();
            cout << (sistema.verificarIntegridad() ? "Decodificacion correcta.\n" : "Error: no coincide con el original.\n");
        }
        else if (opcion == 7) {
            // Resume costos y compara contra codificacion fija de 8 bits.
            sistema.mostrarMetricasYComparacion();
        }
        else if (opcion == 8) {
            cout << "Saliendo...\n";
        }
        else {
            cout << "Opcion no valida.\n";
        }

        if (opcion != 8) {
            cout << "\nPresiona Enter para continuar...";
            cin.get();
            limpiarPantalla();
        }

    } while (opcion != 8);

    return 0;
}

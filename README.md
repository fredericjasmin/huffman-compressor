# Proyecto de Investigacion - Compresor Huffman

## 1. Descripcion formal del problema
El proyecto implementa compresion de texto con Huffman.
Se toma un texto de entrada, se calcula la frecuencia de cada simbolo, se construye el arbol de Huffman, se generan codigos binarios prefijo-libres, se comprime y luego se descomprime para validar que el resultado sea igual al texto original.

## 2. Modelado estructural
El sistema esta dividido por responsabilidades:

- Nodo: representa un nodo del arbol binario.
- MinHeap: cola de prioridad minima para elegir siempre las menores frecuencias.
- CompressorHuffman: flujo principal de frecuencias, arbol, codigos, compresion, descompresion y validaciones.
- Metricas: calculos de bits y porcentaje de compresion.
- main: menu e interaccion con el usuario por consola.

Con este diseño no se concentra toda la logica en main.

## 3. Justificacion de estructuras utilizadas
Estructuras usadas:

- Arbol binario de Huffman (no lineal, obligatorio).
- Min-heap (cola de prioridad, no lineal, obligatorio).
- Hash/map para frecuencias y codigos (obligatorio).
- Vector de caracteres en el proceso de descompresion (lineal).

Alternativa considerada:

- Ordenar listas repetidamente para tomar las menores frecuencias.

Impacto:

- Con min-heap: O(k log k).
- Con ordenamientos repetidos: mas costo total al reordenar varias veces.

Operacion dominante del sistema:

- Construccion del arbol (extracciones e inserciones en heap).

## 4. Algoritmos implementados
Variante A - Huffman estandar:

1. Conteo de frecuencias.
2. Construccion del arbol con min-heap.
3. Recorrido DFS del arbol para generar codigos.
4. Codificacion del texto original.
5. Decodificacion recorriendo el arbol segun cada bit.

Variante B - Comparacion obligatoria:

- Comparacion contra codificacion fija ASCII de 8 bits por caracter.

## 5. Complejidad teorica temporal y espacial
Sea n la longitud del texto y k la cantidad de simbolos distintos.

Temporal:

- Conteo de frecuencias: O(n).
- Construccion del arbol: O(k log k).
- Generacion de codigos (DFS): O(k).
- Compresion: O(n).
- Descompresion: O(n).

Espacial:

- Frecuencias, codigos y arbol: O(k).
- Texto comprimido y descomprimido: O(n).

## 6. Metricas experimentales
El programa reporta:

- Tamano original en bits (ASCII fijo).
- Tamano comprimido en bits (Huffman).
- Porcentaje de compresion.
- Tiempo de compresion en ms.
- Tiempo de descompresion en ms.

Comparacion entre escenarios (corridas reales del equipo):

| Escenario | Texto de prueba | ASCII fijo (bits) | Huffman (bits) | Compresion | Observacion |
|---|---|---:|---:|---:|---|
| Repeticion media (frase con lenguaje natural) | En estructuras de datos usamos arboles y colas para resolver problemas reales | 616 | 290 | 52.92% | Reduccion clara por frecuencias no uniformes en texto natural. |
| Caso corto de control | secret | 48 | 14 | 70.83% | Caso pequeno usado para validacion rapida de flujo. |
| Escenario uniforme | abcdefghijklmnop | 128 | 64 | 50.00% | Huffman asigna codigos de 4 bits por caractér, por eso comprime menos que en textos repetitivos. |

Corrida destacada tomada del programa:

- Texto: En estructuras de datos usamos arboles y colas para resolver problemas reales
- Tamano original: 616 bits
- Tamano comprimido: 290 bits
- Compresion: 52.92%

## 7. Comparacion de enfoques
Enfoque A (fijo): ASCII 8 bits por caracter.

Enfoque B (variable): Huffman.

En la corrida destacada:

- ASCII fijo: 616 bits
- Huffman: 290 bits
- Mejora: Huffman reduce tamano en este caso.

Lectura comparativa por escenarios:

- En textos con alta repeticion, Huffman asigna codigos cortos a los simbolos dominantes y logra la mayor reduccion.
- En textos con repeticion media, la mejora existe pero depende de cuan marcada sea la diferencia de frecuencias.
- En distribuciones mas uniformes, el beneficio tiende a bajar porque no hay simbolos claramente dominantes.

## 8. Analisis de resultados
La mejora depende de la distribucion de frecuencias.
Si hay simbolos muy repetidos, Huffman suele comprimir mejor.
Si la distribucion es uniforme, la mejora baja.

## 8.1 Discusion de escalabilidad
Comportamiento esperado cuando crece el tamano de entrada:

- Si aumenta n (longitud del texto) y k se mantiene pequeno, dominan las fases lineales O(n): contar frecuencias, comprimir y descomprimir.
- Si aumenta k (cantidad de simbolos distintos), crece el costo de construir el arbol por la fase O(k log k).
- En textos muy largos con alfabeto acotado (por ejemplo, texto natural), el sistema escala de forma cercana a lineal en la practica.
- En textos con muchos simbolos distintos, la construccion del arbol y la tabla de codigos gana peso relativo.
- En memoria, el texto de entrada/salida y el bitstream comprimido crecen con O(n), mientras que frecuencias, codigos y arbol crecen con O(k).

Interpretacion practica:

- Para entradas grandes, conviene medir por separado tiempo de construccion y tiempo de codificacion/decodificacion.
- Para comparar escenarios de forma justa, use textos de longitudes similares y cambie solo la distribucion de frecuencias.

## 9. Conclusiones tecnicas
La solucion cumple con:

- Construccion correcta del arbol usando min-heap.
- Codigos prefijo-libres generados por recorrido del arbol.
- Compresion y descompresion con validacion de integridad.
- Comparacion cuantitativa contra codificacion fija.

## 10. Uso de Inteligencia Artificial
Herramienta utilizada:

- GitHub Copilot (GPT-5.3-Codex).

Objetivo del uso:

- Apoyo conceptual para revisar teoria de Huffman, complejidad y criterios de analisis experimental.

Prompt(s) empleados:

- "Explica por que el algoritmo de Huffman produce codigos prefijo-libres y cual es la complejidad de construir el arbol con min-heap."
- "Que escenarios son adecuados para comparar compresion entre codificacion fija de 8 bits y Huffman?"

Resumen de la respuesta:

- Se obtuvieron aclaraciones teoricas sobre prefijo-libre, costo O(k log k) en construccion y criterios para comparar escenarios de prueba.

Como ayudo en la investigacion:

- Ayudo a fundamentar la justificacion tecnica de estructuras y a ordenar el analisis de resultados en el README.

## 11. Validaciones y criterios de prueba
Validaciones implementadas en el sistema:

- Texto vacio: se rechaza con mensaje de error.
- Caso de un solo simbolo: se maneja con construccion especial del arbol.
- Prefijo-libre: se valida la tabla de codigos generada.
- Integridad: se verifica que el texto descomprimido sea exactamente igual al original.

Criterio de prueba de 20 caracteres (aclaracion del enunciado):

- El requisito de minimo 20 caracteres aplica como criterio recomendado para pruebas comparativas, no como restriccion funcional obligatoria del programa.
- La implementacion permite cualquier texto no vacio para facilitar pruebas pequenas y depuracion.
- Para la evaluacion final se recomienda documentar resultados usando entradas de al menos 20 caracteres y, ademas, incluir casos cortos de control.

## Ejecucion
Menu implementado:

1. Ingresar texto
2. Calcular frecuencias
3. Construir arbol Huffman
4. Mostrar codigos
5. Comprimir texto
6. Descomprimir texto
7. Mostrar metricas y comparacion
8. Salir

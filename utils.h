#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <stdint.h>

//Funciones para manejar cadenas de texto estilo C (char*) sin usar la STL.

/**
 * @brief Calcula la longitud de una cadena de texto estilo C.
 * @param str La cadena de entrada (debe terminar en '\0').
 * @return El número de caracteres en la cadena, sin contar el '\0' final.
 */
size_t mi_strlen(const char* str);

/**
 * @brief Copia una cadena de texto a otra.
 * @param destino Puntero al array donde se copiará el texto.
 * @param fuente Puntero a la cadena de origen.
 * @param tamanoMaximo El tamaño máximo del array de destino para evitar desbordamientos.
 */
void mi_strcpy_seguro(char* destino, const char* fuente, size_t tamanoMaximo);

/**
 * @brief Concatena dos cadenas de texto.
 * @param destino Puntero al array de destino. Al final del texto de destino, se pegará el texto de la fuente.
 * @param fuente Puntero a la cadena de origen.
 * @return Un puntero a la cadena de destino.
 */
char* mi_strcat(char* destino, const char* fuente);

/**
 * @brief Busca la primera ocurrencia de una subcadena dentro de una cadena.
 * @param textoGrande La cadena principal donde se buscará.
 * @param textoChiquito La subcadena que se está buscando.
 * @return Un puntero a la primera ocurrencia de la subcadena dentro de la cadena principal, o nullptr si no se encuentra.
 */
char* mi_strstr(const char* textoGrande, const char* textoChiquito);

/**
 * @brief Convierte una secuencia de caracteres que representan dígitos en un número entero.
 * @param str Puntero a la cadena que contiene los dígitos.
 * @param posicion Puntero a un entero que guardará la posición del siguiente carácter después de los dígitos.
 * @return El número entero resultante.
 */
int leerNumero(const char* str, int* posicion);

// -------------------------------------------------------------------------
//                          Algoritmos de Desencriptación
// -------------------------------------------------------------------------

/**
 * @brief Desencripta un bloque de datos aplicando la operacion de Rotación de Bits Cíclica Inversa.
 *
 * La encriptacion fue Rotacion Izquierda n, por lo que la desencriptacion es una
 * Rotacion Derecha (n) a nivel de bits (operadores >> y <<).
 *
 * @param buffer Puntero al bloque de datos a desencriptar.
 * @param tamano El tamaño del bloque de datos en bytes.
 * @param clave La clave de rotación (valor entero, n).
 */
void desencriptarROT(char* buffer, size_t tamano, int clave);

/**
 * @brief Desencripta un bloque de datos usando la operación XOR a nivel de bits.
 *
 * La operación XOR es su propio inverso, lo que significa que aplicar XOR
 * dos veces con la misma clave devuelve el dato original.
 *
 * @param buffer Puntero al bloque de datos a desencriptar.
 * @param tamano El tamaño del bloque de datos en bytes.
 * @param clave La clave de encriptación (un carácter).
 */
void desencriptarXOR(char* buffer, size_t tamano, char clave);

// -------------------------------------------------------------------------
//                          Algoritmos de Descompresión
// -------------------------------------------------------------------------

/**
 * @brief Descomprime un bloque de datos usando el algoritmo RLE (Run-Length Encoding).
 *
 * El RLE se modela como [Basura, repeticiones (1 byte entero), caracter].
 *
 * @param datosComprimidos Puntero a los datos encriptados/comprimidos.
 * @param tamanoArchivo El tamaño final del buffer de entrada.
 * @param tamanoFinal Puntero al tamaño final del buffer de salida.
 * @return Puntero a una nueva cadena de caracteres con los datos descomprimidos. Retorna nullptr si no es un RLE válido.
 */
char* descomprimirRLE(const char* datosComprimidos, size_t tamanoArchivo, size_t* tamanoFinal);

/**
 * @brief Descomprime un bloque de datos usando el algoritmo LZ78.
 * @param datosComprimidos Puntero a los datos encriptados/comprimidos.
 * @param tamanoArchivo El tamaño total del buffer de entrada.
 * @return Puntero a una nueva cadena de caracteres con los datos descomprimidos. Retorna nullptr si no es un LZ78 válido.
 */
char* descomprimirLZ78(const char* datosComprimidos, size_t tamanoArchivo);

#endif // UTILS_H

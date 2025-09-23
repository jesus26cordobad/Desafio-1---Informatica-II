#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

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

#endif // UTILS_H

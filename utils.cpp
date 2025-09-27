#include "utils.h"
#include <iostream>

//Implementación de mi_strlen
size_t mi_strlen(const char* str) {
    size_t contador = 0;
    while (str[contador]!= '\0') {
        contador++;
    }
    return contador;
}

//Implementación de mi_strcpy_seguro
void mi_strcpy_seguro(char* destino, const char* fuente, size_t tamanoMaximo) {
    if (tamanoMaximo == 0) return;
    size_t i = 0;
    while (i < tamanoMaximo - 1 && fuente[i]!= '\0') {
        destino[i] = fuente[i];
        i++;
    }
    destino[i] = '\0';
}

//Implementación de mi_strcat
char* mi_strcat(char* destino, const char* fuente) {
    char* ptr = destino;
    //Mover el puntero al final de la cadena de destino.
    while (*ptr!= '\0') {
        ptr++;
    }
    //Copiar la cadena fuente al final del destino.
    while (*fuente!= '\0') {
        *ptr = *fuente;
        ptr++;
        fuente++;
    }
    //Añadir el terminador nulo.
    *ptr = '\0';
    return destino;
}

//Implementación de mi_strstr
char* mi_strstr(const char* textoGrande, const char* textoChiquito) {
    if (*textoChiquito == '\0') {
        return (char*)textoGrande;
    }

    const char* ptr1 = textoGrande;
    while (*ptr1!= '\0') {
        const char* ptr2 = textoChiquito;
        const char* ptr3 = ptr1;
        while (*ptr2!= '\0' && *ptr3!= '\0' && *ptr2 == *ptr3) {
            ptr2++;
            ptr3++;
        }
        if (*ptr2 == '\0') {
            return (char*)ptr1;
        }
        ptr1++;
    }
    return nullptr;
}

//Implementación de leerNumero
int leerNumero(const char* str, int* posicion) {
    int numero = 0;
    int i = *posicion;
    //Mientras el carácter actual sea un dígito.
    while (str[i] >= '0' && str[i] <= '9') {
        //Convierte el carácter a su valor numérico y lo añade al número.
        //Por ejemplo, '5' - '0' = 5.
        numero = numero * 10 + (str[i] - '0');
        i++;
    }
    //Actualiza la posición para el siguiente carácter después del número.
    *posicion = i;
    return numero;
}

// -------------------------------------------------------------------------
//                          Implementación de Algoritmos de Desencriptación
// -------------------------------------------------------------------------

// Implementación de desencriptarROT
void desencriptarROT(char* buffer, size_t tamano, int n) {
    //n es la clave de rotación
    for (size_t i = 0; i < tamano; ++i) {
        unsigned char byte = (unsigned char)buffer[i];
        // (byte >> n)  ->  Mueve n bits a la derecha (los n bits más bajos se pierden).
        // (byte << (8 - n)) -> Los n bits que se perdieron, se traen de vuelta al inicio.
        buffer[i] = (char)((byte >> n) | (byte << (8 - n)));
    }
}

// Implementación de desencriptarXOR
void desencriptarXOR(char* buffer, size_t tamano, char clave) {
    for (size_t i = 0; i < tamano; ++i) {
        //La operación XOR (^) es su propio inverso.
        buffer[i] = buffer[i] ^ clave;
    }
}

// -------------------------------------------------------------------------
//                          Implementación de Descompresión RLE
// -------------------------------------------------------------------------

char* descomprimirRLE(const char* datosComprimidos, size_t tamanoArchivo ,size_t* tamanoFinal) {
    //Si el archivo no es divisibe por 3, el formato es inválido.
    if (tamanoArchivo % 3 != 0 || tamanoArchivo == 0){
        return nullptr;
    }
    //Se estima un tamaño de salida razonable
    size_t capacidadMaxima = tamanoArchivo * 2;
    char* resultado = new char[capacidadMaxima];
    size_t posResultado = 0;

    //Se recorre el archivo de 3 en 3 bytes.
    for (size_t i = 0; i < tamanoArchivo; i += 3) {

        //Byte 2: longitud de repetición, se lee como un entero sin signo (0-255)
        unsigned char repeticiones = (unsigned char)datosComprimidos[i + 1];

        //Byte 3: carácter a repetir.
        char caracter = datosComprimidos[i + 2];

        //Se repite el carácter la cantidad de veces indicada.
        for (unsigned char j = 0; j < repeticiones; ++j) {
            //redimensionamiento de memoria dinámica
            if (posResultado >= capacidadMaxima - 1) {
                capacidadMaxima *= 2;
                char* nuevoResultado = new char[capacidadMaxima];
                mi_strcpy_seguro(nuevoResultado, resultado, posResultado + 1);
                delete[] resultado;
                resultado = nuevoResultado;
            }
            resultado[posResultado++] = caracter;
        }
    }

    resultado[posResultado] = '\0';
    *tamanoFinal = posResultado;
    return resultado;
}

//Tamaño inicial estimado para el diccionario.
const int LZ78_TAMANO_INICIAL = 1024;

// --------------------------------------------------------------
//          Implementación de Descompresión LZ78
// --------------------------------------------------------------

/**
 * @brief Implementación de la descompresión LZ78.
 * @param datosComprimidos Puntero a los datos encriptados/comprimidos.
 * @param tamanoArchivo El tamaño total del buffer de entrada.
 * @return Puntero a una nueva cadena de caracteres con los datos descomprimidos.
 */

char* descomprimirLZ78(const char* datosComprimidos, size_t tamanoArchivo) {
    if (tamanoArchivo % 3 != 0 || tamanoArchivo == 0) {
        return nullptr;
    }

    //Inicialización diccionario dinámico (char**).
    char** diccionario = new char*[LZ78_TAMANO_INICIAL];
    size_t capacidadDiccionario = LZ78_TAMANO_INICIAL;
    size_t siguienteIndice = 1; //Indice 0 es para el prefijo vacío.

    //Inicializacion punteros a nullptr.
    for(size_t i = 0; i < capacidadDiccionario; i++) {
        diccionario[i] = nullptr;
    }

    //Buffer de salida dinámico
    size_t tamanoSalidaEstimado = tamanoArchivo * 5; //Estimacion
    char* resultado = new char[tamanoSalidaEstimado];
    //Inicializamos el buffer a cero para evitar caracteres basura.
    for (size_t i = 0; i < tamanoSalidaEstimado; ++i) {
        resultado[i] = '\0';
    }

    size_t posResultado = 0;

    //Variables de lógica
    char* prefijoAnterior = nullptr;

    //Bucle principal de descompresión (Lectura de 3 en 3 bytes)
    for (size_t i = 0; i < tamanoArchivo; i += 3) {

        //Lectura del par (índice, caracter)
        unsigned char byteAlto = (unsigned char)datosComprimidos[i];
        unsigned char byteBajo = (unsigned char)datosComprimidos[i + 1];
        char caracter = datosComprimidos[i + 2];

        //Reconstruir el índice de 16 bits (uint16_t)
        unsigned int indice = (unsigned int)(byteAlto * 256 + byteBajo);

        //Determinar la cadena S a escribir
        char* cadenaActual = nullptr;

        if (indice == 0) {
            //Indice 0 es prefijo vacío, la cadena es sólo el carácter.
            //Se crea un nuevo string para el diccionario.
            cadenaActual = new char[2];
            cadenaActual[0] = caracter;
            cadenaActual[1] = '\0';
        } else if (indice < siguienteIndice) {
            //Indice existe en el diccionario.
            //Hay que reconstruir la cadena S: diccionario[indice] + carácter.
            char* prefijo = diccionario[indice - 1]; //El diccionario comienza en 1.

            //Reconstruir la cadena (prefijo + caracter)
            size_t lenPrefijo = mi_strlen(prefijo);
            cadenaActual = new char[lenPrefijo + 2];
            mi_strcpy_seguro(cadenaActual, prefijo, lenPrefijo + 2);
            cadenaActual[lenPrefijo] = caracter;
            cadenaActual[lenPrefijo + 1] = '\0';
        } else {
            goto limpieza_fallo;
        }

        //Escribir S en la salida y aumentar el buffer si es necesario.
        size_t lenCadenaActual = mi_strlen(cadenaActual);
        if (posResultado + lenCadenaActual >= tamanoSalidaEstimado - 1) {
            //Lógica de redimensionamiento de salida
            tamanoSalidaEstimado *= 2;
            char* nuevoResultado = new char[tamanoSalidaEstimado];
            mi_strcpy_seguro(nuevoResultado, resultado, posResultado + 1);
            delete[] resultado;
            resultado = nuevoResultado;
        }

        //Escribir en la salida
        mi_strcat(resultado, cadenaActual);
        posResultado += lenCadenaActual;

        //Insertar S en el diccionario si hay un prefijo anterior
        if (siguienteIndice >= capacidadDiccionario) {
            goto limpieza_fallo;
        }

        //Insertar en el diccionario
        //Si el indice ya existía no se inserta cadenas, solo se inserta la nueva secuencia.
        if (indice == 0 || indice > 0) {
            //El LZ78 inserta la cadena S que acaba de formar.
            //Creamos una copia de S para el diccionario.
            char* copiaParaDiccionario = new char[lenCadenaActual + 1];
            mi_strcpy_seguro(copiaParaDiccionario, cadenaActual, lenCadenaActual + 1);

            diccionario[siguienteIndice - 1] = copiaParaDiccionario;
            siguienteIndice++;
        }

        //Liberar la memoria temporal de cadenaActual
        if (cadenaActual != nullptr) {
            delete[] cadenaActual;
        }
    }

    resultado[posResultado] = '\0';

    //Limpieza final de la memoria del diccionario
    for(size_t i = 0; i < siguienteIndice - 1; ++i) {
        delete[] diccionario[i]; //Liberar cadena
    }
    delete[] diccionario; //Liberar el arreglo de punteros

    return resultado;

//Manejo de fallo de formato o memoria
limpieza_fallo:
    //Limpiar diccionario antes de salir
    for(size_t i = 0; i < siguienteIndice; ++i) {
        if(diccionario[i] != nullptr) delete[] diccionario[i];
    }
    delete[] diccionario;

    //Liberar el buffer de resultado (si existe)
    if (resultado != nullptr) {
        delete[] resultado;
    }
    return nullptr;
}



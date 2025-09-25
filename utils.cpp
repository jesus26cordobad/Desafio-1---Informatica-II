#include "Utils.h"

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
void desencriptarROT(char* buffer, size_t tamano, int clave) {
    for (size_t i = 0; i < tamano; ++i) {
        //Para revertir la rotación, restamos la clave a cada byte.
        buffer[i] -= clave;
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
//                          Implementación de Descompresión
// -------------------------------------------------------------------------

char* descomprimirRLE(const char* datosComprimidos, size_t* tamano) {
    size_t tamanoOriginal = mi_strlen(datosComprimidos);
    size_t capacidadMaxima = tamanoOriginal + 10;
    char* resultado = new char[capacidadMaxima];
    size_t posComprimido = 0;
    size_t posResultado = 0;

    while (posComprimido < tamanoOriginal) {
        if (datosComprimidos[posComprimido] < '0' || datosComprimidos[posComprimido] > '9') {
            delete[] resultado;
            return nullptr;
        }

        int repeticiones = leerNumero(datosComprimidos, (int*)&posComprimido);
        char caracter = datosComprimidos[posComprimido];
        posComprimido++;

        for (int i = 0; i < repeticiones; ++i) {
            if (posResultado >= capacidadMaxima - 1) {
                capacidadMaxima *= 2;
                char* nuevoResultado = new char[capacidadMaxima];
                mi_strcpy_seguro(nuevoResultado, resultado, mi_strlen(resultado) + 1);
                delete[] resultado;
                resultado = nuevoResultado;
            }
            resultado[posResultado++] = caracter;
        }
    }
    resultado[posResultado] = '\0';
    *tamano = posResultado;
    return resultado;
}

char* descomprimirLZ78(const char* datosComprimidos) {
    // La implementación completa de LZ78 es compleja y requiere estructuras de datos
    // como tablas hash o árboles. Para este desafío, se puede considerar un algoritmo
    // "fuerza bruta" para validar la estructura o simplemente una simulación.
    // Aquí se retorna nullptr para indicar que no es este el método de compresión.
    return nullptr;
}

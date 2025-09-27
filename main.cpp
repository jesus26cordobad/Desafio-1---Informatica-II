#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <limits>
#include <string>
#include "utils.h"

// ------------------------------------------------------
//                  Funciones Auxiliares
// ------------------------------------------------------

/**
 * @brief Carga el contenido completo de un archivo binario.
 * @param nombreArchivo El nombre del archivo a cargar.
 * @param tamano Puntero donde se almacenará el tamaño del buffer cargado.
 * @return Un puntero a un nuevo array de char con el contenido del archivo, o nullptr si falla.
 */

char* cargarArchivo(const std::string& nombreArchivo, size_t* tamano) {
    // Usamos c_str() para que fstream use un puntero char*
    std::ifstream archivoEntrada(nombreArchivo.c_str(), std::ios::binary | std::ios::ate);
    if (!archivoEntrada.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << std::endl;
        *tamano = 0;
        return nullptr;
    }

    *tamano = archivoEntrada.tellg();
    archivoEntrada.seekg(0, std::ios::beg);
    char* datos = new char[*tamano + 1];
    archivoEntrada.read(datos, *tamano);
    datos[*tamano] = '\0';
    archivoEntrada.close();
    return datos;
}

// ------------------------------------------------------
//                  Función Principal
// ------------------------------------------------------

int main() {
    int numDatasets;
    std::cout << "Ingrese el numero de datasets (casos de prueba) a evaluar: ";
    //Evitamos el uso de try-catch para mantener la simplicidad.
    if (!(std::cin >> numDatasets) || numDatasets <= 0 || numDatasets > 4) {
        std::cerr << "Entrada invalida o fuera de rango (1-4). Saliendo." << std::endl;
        return 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    //Bucle para automatizar el testing
    for (int caso = 1; caso <= numDatasets; ++caso) {
        std::cout << "\n========================================================" << std::endl;
        std::cout << "        INICIANDO CASO DE PRUEBA #" << caso << std::endl;
        std::cout << "========================================================" << std::endl;

        // --- Carga de Archivos ---
        // Construimos el nombre del archivo
        std::string nombreEncriptado = "Encriptado" + std::to_string(caso) + ".txt";
        std::string nombrePista = "pista" + std::to_string(caso) + ".txt";

        size_t tamanoArchivo = 0;
        char* datosOriginales = cargarArchivo(nombreEncriptado, &tamanoArchivo);
        if (datosOriginales == nullptr) continue; //Si falla la carga, pasamos al siguiente caso.

        size_t tamanoPista = 0;
        char* fragmentoConocido = cargarArchivo(nombrePista, &tamanoPista);
        if (fragmentoConocido == nullptr) {
            delete[] datosOriginales;
            continue;
        }

        //Bucle de fuerza bruta
        bool solucionEncontrada = false;

        for (int k = 0; k < 256 && !solucionEncontrada; ++k) {
            for (int n = 1; n <= 7 && !solucionEncontrada; ++n) {

                // --- PROCESO DE DESENCRIPTACION ---
                char* copiaDatos = new char[tamanoArchivo + 1];
                mi_strcpy_seguro(copiaDatos, datosOriginales, tamanoArchivo + 1);

                //Aplicamos las operaciones inversas en orden inverso a la encriptación.
                //Encriptación: Rot Izq (n) -> XOR (K)
                //Desencriptación: XOR (K) -> Rot Der (n)
                desencriptarXOR(copiaDatos, tamanoArchivo, (char)k);
                desencriptarROT(copiaDatos, tamanoArchivo, n);

                // --- INTENTAR RLE ---
                size_t tamanoDescomprimido = 0;
                //Se pasa tamanoArchivo para la validación de la terna.
                char* resultadoRLE = descomprimirRLE(copiaDatos, tamanoArchivo, &tamanoDescomprimido);

                if (resultadoRLE != nullptr && mi_strstr(resultadoRLE, fragmentoConocido) != nullptr) {
                    // ¡SOLUCION RLE ENCONTRADA!
                    solucionEncontrada = true;
                    //std::cout << "\n--------------------------------------------------------" << std::endl;
                    //std::cout << "        ¡SOLUCION ENCONTRADA!" << std::endl;
                    //std::cout << "--------------------------------------------------------" << std::endl;
                    std::cout << "Mensaje Original Reconstruido:" << std::endl;
                    std::cout << resultadoRLE << std::endl;
                    std::cout << "\nMetodo de Compresion: RLE" << std::endl;
                    std::cout << "Rotacion: " << n << std::endl;
                    std::cout << "Key: " << (int)k << " (0x" << std::hex << (int)k << std::dec << ")" << std::endl;

                    delete[] resultadoRLE;
                    delete[] copiaDatos;
                    // Se rompe el bucle de rotación (n) y el bucle XOR (k)
                    break;
                }

                //Limpieza de memoria para la iteración fallida:
                //Solo liberamos si resultadoRLE fue creado (no es nullptr).
                if (resultadoRLE != nullptr) {
                    delete[] resultadoRLE;
                }

                // --- INTENTAR LZ78 ---
                //Si RLE falló, probamos LZ78.
                char* resultadoLZ78 = descomprimirLZ78(copiaDatos, tamanoArchivo);

                if (resultadoLZ78 != nullptr && mi_strstr(resultadoLZ78, fragmentoConocido) != nullptr) {
                    // ¡SOLUCIÓN LZ78 ENCONTRADA!
                    solucionEncontrada = true;
                    //std::cout << "\n--------------------------------------------------------" << std::endl;
                    //std::cout << "        ¡SOLUCION ENCONTRADA!" << std::endl;
                    //std::cout << "--------------------------------------------------------" << std::endl;
                    std::cout << "Mensaje Original Reconstruido:" << std::endl;
                    std::cout << resultadoLZ78 << std::endl;
                    std::cout << "\nMetodo de Compresion: LZ78" << std::endl;
                    std::cout << "Rotacion: " << n << std::endl;
                    std::cout << "Key: " << (int)k << " (0x" << std::hex << (int)k << std::dec << ")" << std::endl;

                    delete[] resultadoLZ78;
                    delete[] copiaDatos;
                    break;
                }

                //Limpieza después del intento LZ78 (si falló)
                if (resultadoLZ78 != nullptr) {
                    delete[] resultadoLZ78;
                }

                //Limpieza de memoria para la iteración fallida
                delete[] copiaDatos;
            }
        }

        if (!solucionEncontrada) {
            std::cout << "\n--------------------------------------------------------" << std::endl;
            std::cout << "El programa no encontro la solucion con RLE ni LZ78 para el caso #" << caso << "." << std::endl;
            std::cout << "Conclusion: El fragmento conocido no se encontro con ningun metodo/parametro." << std::endl;
            std::cout << "--------------------------------------------------------" << std::endl;
        }

        // Limpieza final de los datos cargados para el caso
        delete[] datosOriginales;
        delete[] fragmentoConocido;
    }

    return 0;
}

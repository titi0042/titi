#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t tipo;           // Bytes 0-1: 'BM'
    uint32_t tamArchivo;     // Bytes 2-5: Tamaño total del archivo
    uint16_t reservado1;     // Bytes 6-7: Reservado (0)
    uint16_t reservado2;     // Bytes 8-9: Reservado (0)
    uint32_t offsetDatos;    // Bytes 10-13: Inicio de los datos de imagen
} BMPFileHeader;

typedef struct {
    uint32_t tamCabecera;    // Bytes 14-17: Tamaño de este header (40)
    int32_t  ancho;          // Bytes 18-21: Ancho en píxeles
    int32_t  alto;           // Bytes 22-25: Alto en píxeles
    uint16_t nPlanos;        // Bytes 26-27: Número de planos (1)
    uint16_t bpp;            // Bytes 28-29: Bits por píxel (24)
    uint32_t compresion;     // Bytes 30-33: 0 si no está comprimido
    uint32_t tamImagen;      // Bytes 34-37: Tamaño de la imagen en bytes
    int32_t  resHorizontal;  // Bytes 38-41: Resolución horizontal
    int32_t  resVertical;    // Bytes 42-45: Resolución vertical
    uint32_t nColores;       // Bytes 46-49: Número de colores en la tabla (0 para 24 bits)
    uint32_t colImport;      // Bytes 50-53: Número de colores importantes (0 = todos)
} BMPInfoHeader;

typedef struct {
 uint8_t b;
 uint8_t g;
 uint8_t r;
} PIXEL;

#pragma pack(pop)

void liberarMatriz(PIXEL** matriz, size_t filas);
PIXEL** crearMatriz( size_t filas, size_t columnas);
void llenarMatriz(PIXEL **matriz, size_t filas, size_t columnas, FILE *foto);
void mostrarMatriz(PIXEL **matriz, size_t filas, size_t columnas);
uint8_t* crearvector( size_t ce);
void EscaladeGrises(PIXEL **matriz, size_t filas, size_t columnas);



#endif // FUNCIONES_GRUPO_H_INCLUDED

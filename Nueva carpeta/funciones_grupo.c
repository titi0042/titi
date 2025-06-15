#include "funciones_grupo.h"
void liberarvector(uint8_t* vector)
{
    free(vector);
}

uint8_t* crearvector( size_t ce)
{
    uint8_t *vector = (uint8_t*)malloc(ce * sizeof(uint8_t));

    if(vector == NULL)
    {
        return NULL;
    }
    return vector;
}

void liberarMatriz(PIXEL** matriz, size_t filas)
{
    size_t i;
    for(i = 0 ; i < filas ; i++)
        free(matriz[i]);
    free(matriz);
}
PIXEL** crearMatriz( size_t filas, size_t columnas)
{
    size_t i;

    PIXEL **matriz = (PIXEL**)malloc(filas * sizeof(PIXEL*));

    if(matriz == NULL)
    {
        return NULL;
    }
    for(i = 0 ; i < filas ; i++)
    {
        *(matriz+i) = (PIXEL*)malloc(columnas * sizeof(PIXEL));

        if(*(matriz+i) == NULL)
        {
            liberarMatriz(matriz,i);
            return NULL;
        }
    }

    return matriz;

}
void llenarMatriz(PIXEL **matriz, size_t filas, size_t columnas, FILE *foto)
{
    size_t i, j, padding, fila_bytes;
    uint8_t bytes[3];
    fila_bytes = 3 * columnas;
    padding = (4 - (fila_bytes % 4)) % 4;



        for(i = 0 ; i < filas; i++)
        {
            for(j = 0 ; j < columnas ; j++)
            {
                fread(bytes,1,3,foto);
                matriz[i][j].b = bytes[0];
                matriz[i][j].g = bytes[1];
                matriz[i][j].r = bytes[2];

            }
                fseek(foto, padding, SEEK_CUR);
        }

}
void mostrarMatriz(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            printf("%02x\t",matriz[i][j].b);
            printf("%02x\t",matriz[i][j].g);
            printf("%02x\t",matriz[i][j].r);
        }
        printf("\n");
    }
}
//////////////////////////////////////////CREAR IMAGEN//////////////////////////////////////////////////////
void CrearImagen(PIXEL **matriz,BMPInfoHeader ih,BMPFileHeader fh,char nombre_imagen,uint8_t* vecext)
{
    uint8_t byte;
    size_t i,j,k;
    PIXEL padding;
    FILE *nuevaimagen;
    padding.b=0;
    padding.g=0;
    padding.r=0;
    k = (3 * ih.ancho) % 4;

    nuevaimagen=fopen("escala_de_grises.bmp","wb");
    if (!nuevaimagen)
    {
        perror("No se pudo abrir el archivo");
        return EXIT_FAILURE;
    }
    fwrite(&fh,sizeof(BMPFileHeader),1,nuevaimagen);

    fwrite(&ih,sizeof(BMPInfoHeader),1,nuevaimagen);

    if(ih.tamCabecera>40)
    {
        fwrite(vecext,sizeof(uint8_t),ih.tamCabecera-40,nuevaimagen);
    }

    for(i = 0 ; i < ih.alto ; i++)
    {
        for(j = 0 ; j < ih.ancho ; j++)
        {
            byte=matriz[i][j].b;
            fwrite(&byte,1,1,nuevaimagen);

            byte=matriz[i][j].g;
            fwrite(&byte,1,1,nuevaimagen);

            byte=matriz[i][j].r;
            fwrite(&byte,1,1,nuevaimagen);

        }

        fwrite(&padding,sizeof(PIXEL),1,nuevaimagen);

    }
    printf("imagen creada");
    fclose(nuevaimagen);
}
/////////////////////////////////////////NEGATIVO//////////////////////////////////////////////////////////////////////
void Negativo(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            matriz[i][j].b=255-matriz[i][j].b;
            matriz[i][j].g=255-matriz[i][j].g;
            matriz[i][j].r=255-matriz[i][j].r;
        }

    }

}
//////////////////////////////////////////ESCALA DE GRISES///////////////////////////////////////////////
void EscaladeGrises(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j;
    uint8_t aux;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            aux = (matriz[i][j].b+matriz[i][j].g+matriz[i][j].r)/3;
            matriz[i][j].b=aux;
            matriz[i][j].g=aux;
            matriz[i][j].r=aux;
        }

    }

}

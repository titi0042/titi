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
    size_t i, j,k, padding, fila_bytes;
    uint8_t bytes;
    fila_bytes = 3 * columnas;
    padding = (4 - (fila_bytes % 4)) % 4;



        for(i = 0 ; i < filas; i++)
        {
            for(j = 0 ; j < columnas ; j++)
            {
                for(k = 0 ; k < 3 ; k++)
                {
                fread(&bytes,1,1,foto);
                matriz[i][j].bgr[k] = bytes;
                }

            }
                fseek(foto, padding, SEEK_CUR);
        }

}
void mostrarMatriz(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j, k;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
            printf("%02x\t",matriz[i][j].bgr[k]);
            }
        }
        printf("\n");
    }
}
//////////////////////////////////////////CREAR IMAGEN//////////////////////////////////////////////////////
void CrearImagen(PIXEL **matriz,BMPInfoHeader ih,BMPFileHeader fh,char nombre_imagen,uint8_t* vecext)
{
    uint8_t byte,cero=0;
    size_t i,j,k,padding;
    FILE *nuevaimagen;
    padding = (4-(3*ih.ancho)% 4)%4;

    nuevaimagen=fopen("rotar_dere.bmp","wb");
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
            for(k = 0 ; k < 3 ; k++)
            {
            byte=matriz[i][j].bgr[k];
            fwrite(&byte,1,1,nuevaimagen);
            }
        }
        for(k=0;k<padding;k++)
        {
          fwrite(&cero,sizeof(cero),1,nuevaimagen);
        }
    }
    printf("imagen creada");
    fclose(nuevaimagen);
}
/////////////////////////////////////////NEGATIVO//////////////////////////////////////////////////////////////////////
void Negativo(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j, k;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
            matriz[i][j].bgr[k]=255-matriz[i][j].bgr[k];
            }
        }

    }

}
//////////////////////////////////////////ESCALA DE GRISES///////////////////////////////////////////////
void EscaladeGrises(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j, k;
    uint8_t aux=0;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                aux +=matriz[i][j].bgr[k];
            }
            aux=aux/3;
            for(k = 0 ; k < 3 ; k++)
            {
                matriz[i][j].bgr[k];
            }
        }

    }

}
///////////////////////////////////////ESPEJAR HORIZONTAL////////////////////////////////////////////////
void EspejarHorizontal(PIXEL **matriz, size_t fin_fi, size_t columnas)
{
    size_t i, j, k, fin_col;
    PIXEL aux;

    for(i = 0 ; i <fin_fi ; i++)
    {
        fin_col=columnas-1;
        for(j = 0 ; j < fin_col ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                aux.bgr[k]=matriz[i][j].bgr[k];

                matriz[i][j].bgr[k]=matriz[i][fin_col].bgr[k];

                matriz[i][fin_col].bgr[k]=aux.bgr[k];
            }
            fin_col--;

        }

    }

}
//////////////////////////////////ESPEJADO VERTICAL//////////////////////////////////////////////////
void EspejarVertical(PIXEL **matriz, size_t filas, size_t columnas)
{
    size_t i, j, k, fin_fi;
    PIXEL aux;

    for(i = 0 ; i <columnas ; i++)
    {
        fin_fi=filas-1;
        for(j = 0 ; j < fin_fi ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                aux.bgr[k]=matriz[j][i].bgr[k];

                matriz[j][i].bgr[k]=matriz[fin_fi][i].bgr[k];

                matriz[fin_fi][i].bgr[k]=aux.bgr[k];
            }
        fin_fi--;

        }
    }
}
/////////////////////////////////AUMENTAR CONTRASTE/////////////////////////////////
void AumentarContraste(PIXEL **matriz, size_t filas, size_t columnas,float factor)
{

    size_t i, j, k;
    float nuevo_valor=0;
    factor=factor/100.0+1.0;
    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {

            for(k = 0 ; k < 3 ; k++)
            {
                nuevo_valor =128.0+(factor*((float)matriz[i][j].bgr[k]-128.0));
                if (nuevo_valor <= 0)
                {
                    nuevo_valor = 0;
                }
                if (nuevo_valor > 255)
                {
                nuevo_valor = 255;
                }
                matriz[i][j].bgr[k] = nuevo_valor;
            }

        }

    }
}
/////////////////////////////REDUCIR CONTRASTE////////////////////////////////////////
void ReducirContraste(PIXEL **matriz, size_t filas, size_t columnas,float factor)
{
    printf("%f",factor);
    size_t i, j, k;
    float nuevo_valor=0;
    factor =factor/100.0;
    printf("%d",matriz[0][0].bgr[0] );
    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < columnas ; j++)
        {

            for(k = 0 ; k < 3 ; k++)
            {
                nuevo_valor =128.0+(1.0-factor)*((float)matriz[i][j].bgr[k]-128.0);
                if (nuevo_valor <= 0)
                {
                    nuevo_valor = 0;
                }
                if (nuevo_valor > 255)
                {
                nuevo_valor = 255;
                }
                matriz[i][j].bgr[k] = nuevo_valor;
            }

        }

    }
    printf("%d",matriz[0][0].bgr[0] );
}
////////////////////////////////////////ACHICAR////////////////////////////////////////////////
PIXEL** AchicarMatriz(PIXEL **matriz, size_t filas, size_t columnas, float factor)
{
    PIXEL **matriz_reducida;
    int ancho_redu, alto_redu, x, y, x1, y1,i,j,k;
    float escala_x, escala_y;
    ancho_redu=columnas*(1-factor);
    alto_redu=filas*(1-factor);
    printf("%d",alto_redu);
    matriz_reducida=crearMatriz(alto_redu,ancho_redu);
    escala_x=columnas/ancho_redu;
    escala_y=filas/alto_redu;
    for(i = 0 ; i < alto_redu ; i++)
    {
        y = (int)escala_y* i;
        y1 = (y+1<columnas-1)?y+1:filas-1;
        for(j = 0 ; j < ancho_redu ; j++)
        {
            x = (int)escala_x* j;
            x1 = (x+1<columnas-1)?x+1:columnas-1;

            for(k = 0 ; k < 3 ; k++)
            {

                matriz_reducida[i][j].bgr[k]=(matriz[y][x].bgr[k] + matriz[y][x1].bgr[k] + matriz[y1][x].bgr[k] + matriz[y1][x1].bgr[k]) / 4;

            }

        }

    }


    return matriz_reducida;
}
////////////////////////////////ROTAR DERECHA////////////////////////////////
PIXEL** RotarDerecha(PIXEL **matriz,size_t filas, size_t columnas)
{

        int i, j;
        PIXEL** nueva_matriz = crearMatriz(columnas, filas);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
            nueva_matriz[j][i] = matriz[i][j];
            }
        }

        return nueva_matriz;

}
////////////////////////////////ROTAR IZQUIERDA////////////////////////////////
PIXEL** RotarIzquierda(PIXEL **matriz,size_t filas, size_t columnas)
{

        int i, j;
        PIXEL** nueva_matriz = crearMatriz(columnas, filas);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
            nueva_matriz[j][filas - i - 1] = matriz[i][j];
            }
        }

        return nueva_matriz;

}

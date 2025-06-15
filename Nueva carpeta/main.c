#include "funciones_grupo.h"

int main(int argc, char *argv[])
{
    PIXEL **matrizpixeles;
    uint8_t *vector=0, byte;
    size_t filas, columnas;
    int i = 0,j=0, n=1;
    int extended_header = 0;

    /*if (argc < 2) {
        printf("Error: Se deben pasar al menos un argumento.\n");
        return 1;
    }*/
    uint8_t padding=0;

    FILE *archivo, *nuevaimagen;

    archivo = fopen("unlam_359.bmp", "rb");
    if (!archivo)
    {
        perror("No se pudo abrir el archivo");
        return EXIT_FAILURE;
    }

    BMPFileHeader fh;
    BMPInfoHeader ih;

    fread(&fh, sizeof(BMPFileHeader), 1, archivo);
    fread(&ih, sizeof(BMPInfoHeader), 1, archivo);

    // Validar que sea un BMP de 24 bits
    if (fh.tipo != 0x4D42 || ih.bpp != 24)
    {
        fprintf(stderr, "Archivo no es un BMP de 24 bits.\n");
        fclose(archivo);
        return EXIT_FAILURE;
    }

    printf("Imagen: %dx%d, %d bits por pixel\n", ih.ancho, ih.alto, ih.bpp);
// calculo si es negativo el alto y ancho
    if(ih.alto>0)
    {
        filas=ih.alto;
    }
    else
    {
      filas= -ih.alto;
    }
    if(ih.ancho>0)
        columnas=ih.ancho;
    else
        exit(1);
// verificar el tamaño del header por si tiene un header extendido y guardarlo.
    if(ih.tamCabecera>39)
    {
        vector=crearvector(ih.tamCabecera-40);
        extended_header=1;
        fread(vector,sizeof(vector),1,archivo);

    }
    matrizpixeles = crearMatriz(filas,columnas);
    llenarMatriz(matrizpixeles,filas,columnas,archivo);
    if (n==1)
        {

            EscaladeGrises(matrizpixeles,filas,columnas);
            nuevaimagen=fopen("escala_de_grises.bmp","wb");
            if (!archivo)
            {
                perror("No se pudo abrir el archivo");
                return EXIT_FAILURE;
            }
            fwrite(&fh,sizeof(BMPFileHeader),1,nuevaimagen);

            fwrite(&ih,sizeof(BMPInfoHeader),1,nuevaimagen);

            if(extended_header==1)
            {
                fwrite(vector,sizeof(vector),1,nuevaimagen);
            }
            for(i = 0 ; i < filas ; i++)
            {
                for(j = 0 ; j < columnas ; j++)
                {
                    byte=matrizpixeles[i][j].b;
                    fwrite(&byte,1,1,nuevaimagen);

                    byte=matrizpixeles[i][j].g;
                    fwrite(&byte,1,1,nuevaimagen);

                    byte=matrizpixeles[i][j].r;
                    fwrite(&byte,1,1,nuevaimagen);


                }
                fwrite(padding,1,1,nuevaimagen);
                fwrite(padding,1,1,nuevaimagen);
                fwrite(padding,1,1,nuevaimagen);

            }
            printf("imagen creada");
            fclose(nuevaimagen);
            n=0;

        }
   /* for (int i = 1; i < 2; i++)
    {
        if (strcmp(argv[i], "--negativo") == 0)
        {
            negativo = 1;
        }

        if (strcmp(argv[i], "--espejar-horizontal") == 0)
        {
            espejar_horizontal = 1;
        }
        if (strcmp(argv[i], "--espejar-vertical") == 0)
        {
            espejar_vertical = 1;
        }
        if (strncmp(argv[i], "--aumentar-contraste=", 21) == 0)
        {
            aumentar_contraste = 1;
        }
        if (strncmp(argv[i], "--reducir-contraste=", 20) == 0)
        {
            reducir_contraste = 1;
        }
        if (strncmp(argv[i], "--tonalidad-azul=", 16) == 0)
        {
            tonalidad_azul = 1;
        }
        if (strncmp(argv[i], "--tonalidad-verde=", 27) == 0)
        {
            tonalidad_verde = 1;
        }
        if (strncmp(argv[i], "--tonalidad-roja=", 16) == 0)
        {
            tonalidad_roja = 1;
        }
        if (strncmp(argv[i], "--recortar=", 11) == 0)
        {
            recortar = 1;
        }
        if (strncmp(argv[i], "--achicar=", 10) == 0)
        {
            achicar = 1;
        }
        if (strcmp(argv[i], "--rotar-derecha") == 0)
        {
            rotar_derecha = 1;
        }
        if (strcmp(argv[i], "--rotar-izquierda") == 0)
        {
            rotar_izquierda = 1;
        }
        if (strcmp(argv[i], "--concatenar-horizontal") == 0)
        {
            concatenar_horizontal = 1;
        }
        if (strcmp(argv[i], "--concatenar-vertical") == 0)
        {
            concatenar_vertical = 1;
        }
        if (strcmp(argv[i], "--comodin") == 0)
        {
            comodin = 1;
        }

    }*/
    //mostrarMatriz(matrizpixeles,1,columnas);
    liberarMatriz(matrizpixeles, filas);
    fclose(archivo);
    liberarvector(vector);
    return EXIT_SUCCESS;
}

#include "funciones_grupo.h"

int main(int argc, char *argv[])
{
    IMAGEN foto, foto2;
    PIXEL **matrizpixeles, **matriz_aux;
    FILE *archivo, *archivo1;
    uint8_t *vector=0, *vector1=0;
    size_t filas, columnas, filas1, columnas1;
    char nombre[50] = "tu_hermana_parada.bmp";
    float factor=50.0f;
    int i = 0,j=0, n=1;
    int extended_header = 0;

    /*if (argc < 2) {
        printf("Error: Se deben pasar al menos un argumento.\n");
        return 1;
    }*/


    archivo = fopen("unlam_359.bmp", "rb");
    if (!archivo)
    {
        perror("No se pudo abrir el archivo");
        return EXIT_FAILURE;
    }





    fread(&foto.cab_file, sizeof(BMPFileHeader), 1, archivo);
    fread(&foto.cab_info, sizeof(BMPInfoHeader), 1, archivo);



    // Validar que sea un BMP de 24 bits

    if (foto.cab_file.tipo != 0x4D42 || foto.cab_info.bpp != 24)
    {
        fprintf(stderr, "Archivo no es un BMP de 24 bits.\n");
        fclose(archivo);
        return EXIT_FAILURE;
    }

    printf("Imagen: %dx%d, %d bits por pixel\n", foto.cab_info.ancho, foto.cab_info.alto, foto.cab_info.bpp);
// calculo si es negativo el alto y ancho
    if(foto.cab_info.alto>0)
    {
        filas=foto.cab_info.alto;
    }
    else
    {
      filas= -foto.cab_info.alto;
    }
    if(foto.cab_info.ancho>0)
        columnas=foto.cab_info.ancho;
    else
        exit(1);
// verificar el tamaño del header por si tiene un header extendido y guardarlo.
    if(foto.cab_info.tamCabecera>40)
    {
        vector=crearvector(foto.cab_info.tamCabecera-40);
        extended_header=1;
        fread(vector,1,foto.cab_info.tamCabecera-40,archivo);
        foto.vect=vector;


    }
    foto.pixeles=crearMatriz(filas,columnas);

    llenarMatriz(foto,archivo);


    /////////////////////////////FOTO 2/////////////////////
    archivo1 = fopen("unlam_1.bmp", "rb");
    if (!archivo1)
    {
        perror("No se pudo abrir el archivo");
        return EXIT_FAILURE;
    }

    fread(&foto2.cab_file, sizeof(BMPFileHeader), 1, archivo1);
    fread(&foto2.cab_info, sizeof(BMPInfoHeader), 1, archivo1);

    if (foto2.cab_file.tipo != 0x4D42 || foto2.cab_info.bpp != 24)
    {
        fprintf(stderr, "Archivo no es un BMP de 24 bits.\n");
        fclose(archivo1);
        return EXIT_FAILURE;
    }

    printf("Imagen: %dx%d, %d bits por pixel\n", foto2.cab_info.ancho, foto2.cab_info.alto, foto2.cab_info.bpp);
// calculo si es negativo el alto y ancho
    if(foto2.cab_info.alto>0)
    {
        filas1=foto2.cab_info.alto;
    }
    else
    {
      filas1= -foto2.cab_info.alto;
    }
    if(foto2.cab_info.ancho>0)
        columnas1=foto2.cab_info.ancho;
    else
        exit(1);
// verificar el tamaño del header por si tiene un header extendido y guardarlo.
    if(foto2.cab_info.tamCabecera>40)
    {
        vector1=crearvector(foto2.cab_info.tamCabecera-40);
        extended_header=1;
        fread(vector1,1,foto2.cab_info.tamCabecera-40,archivo1);
        foto2.vect=vector1;


    }
    foto2.pixeles=crearMatriz(filas1,columnas1);

    llenarMatriz(foto2,archivo1);

    //////////////////////////////////////////////



/*
    if (n==1)
        {

            Negativo(foto,nombre);
            n=0;
        }


    if (n==1)
        {
            EscaladeGrises(foto,nombre);
            n=0;
        }

    if (n==1)
        {
            EspejarHorizontal(foto,nombre);

            n=0;
        }


    if (n==1)
        {
            EspejarVertical(foto,nombre);
            n=0;
        }


    if (n==1)
        {
            AumentarContraste(foto,nombre,factor);

            n=0;
        }


    if (n==1)
        {
            ReducirContraste(foto,nombre,factor);
            n=0;
        }


    if (n==1)
        {

            AchicarImagen(foto,nombre,factor);
            n=0;
        }
        */
        if (n==1)
        {

            ConcatenarHorizontal(foto,foto2,nombre);
            n=0;
        }
/*
    for(n=1)
        {
            matriz_aux=RotarDerecha(matrizpixeles,filas,columnas);
            ih_r=ih;
            ih_r.alto=ih.ancho;
            ih_r.ancho=ih.alto;
            CrearImagen(matriz_aux,ih_r,fh,argv[n],vector);
            n=0;
        }
        */

   /* for (int i = 1; i < argc; i++)
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
    liberarMatriz(foto.pixeles,filas);
    fclose(archivo1);
    fclose(archivo);
    liberarvector(vector);
    return EXIT_SUCCESS;
}

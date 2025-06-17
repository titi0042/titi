#include "funciones_grupo.h"

int main(int argc, char *argv[])
{
    IMAGEN foto, foto2;
    FILE *archivo, *archivo1;
    uint8_t *vector=0, *vector1=0;
    size_t filas, columnas, filas1, columnas1;
    char nombre[50] = "primera_prueba.bmp";
    int factor=0;
    int i = 0,band=0;

    if (argc < 2) {
        printf("Error: Se deben pasar al menos un argumento.\n");
        return 1;
    }


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
        fread(vector,1,foto.cab_info.tamCabecera-40,archivo);
        foto.vect=vector;


    }
    foto.pixeles=crearMatriz(filas,columnas);

    llenarMatriz(foto,archivo);


    /////////////////////////////FOTO 2/////////////////////
    archivo1 = fopen(argv[2], "rb");
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

        fread(vector1,1,foto2.cab_info.tamCabecera-40,archivo1);
        foto2.vect=vector1;


    }
    foto2.pixeles=crearMatriz(filas1,columnas1);

    llenarMatriz(foto2,archivo1);

    //////////////////////////////////////////////


    for (i = 1; i < argc; i++)
    {
        factor=0;
        if (strcmp(argv[i], "--negativo") == 0)
        {
            Negativo(foto,nombre);
        }
        else if(strcmp(argv[i], "--escala-de-grises") == 0)
        {
            EscaladeGrises(foto,nombre);
        }
        else if (strcmp(argv[i], "--espejar-horizontal") == 0)
        {
            EspejarHorizontal(foto,nombre);
        }
        else if (strcmp(argv[i], "--espejar-vertical") == 0)
        {
            EspejarVertical(foto,nombre);
        }
        else if (strncmp(argv[i], "--aumentar-contraste=", 21) == 0)
        {
            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
            AumentarContraste(foto,nombre,factor);
            }
        }
        else if (strncmp(argv[i], "--reducir-contraste=", 20) == 0)
        {
            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
            ReducirContraste(foto,nombre,factor);
            }
        }
        else if (strncmp(argv[i], "--tonalidad-azul=", 16) == 0)
        {
            TonalidadAzul(foto,nombre,factor);
        }
        else if (strncmp(argv[i], "--tonalidad-verde=", 17) == 0)
        {

            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
                TonalidadVerde(foto,nombre,(float)factor);
            }
        }
        else if (strncmp(argv[i], "--tonalidad-roja=", 16) == 0)
        {
            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
            TonalidadRoja(foto,nombre,factor);
            }
        }
        else if (strncmp(argv[i], "--recortar=", 11) == 0)
        {
            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
            recortarImagen(foto,nombre,factor);
            }
        }
        else if (strncmp(argv[i], "--achicar=", 10) == 0)
        {
            band=extraerNumeroDesdeIgual(argv[i],&factor);
            if(band)
            {
            AchicarImagen(foto,nombre,factor);
            }
        }
        else if (strcmp(argv[i], "--rotar-derecha") == 0)
        {
            RotarDerecha(foto,nombre);
        }
        else if (strcmp(argv[i], "--rotar-izquierda") == 0)
        {
            RotarIzquierda(foto,nombre);
        }
        else if (strcmp(argv[i], "--concatenar-horizontal") == 0)
        {
            ConcatenarHorizontal(foto,foto2,nombre);
        }
        else if (strcmp(argv[i], "--concatenar-vertical") == 0)
        {
            ConcatenarVertical(foto,foto2,nombre);
        }
        else if (strcmp(argv[i], "--comodin") == 0)
        {
            Comodin();
        }

    }

    //mostrarMatriz(matrizpixeles,1,columnas);
    liberarMatriz(foto.pixeles,filas);
    fclose(archivo1);
    fclose(archivo);
    liberarvector(vector);
    return EXIT_SUCCESS;
}

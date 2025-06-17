#include "funciones_grupo.h"
void liberarvector(uint8_t* vect)
{
    free(vect);
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
PIXEL** crearMatriz(size_t filas, size_t columnas)
{
    size_t i;
    PIXEL **matriz = (PIXEL**)malloc(filas* sizeof(PIXEL*));

    if(matriz == NULL)
    {
        return NULL;
    }
    for(i = 0 ; i < filas ; i++)
    {
        *(matriz+i) = (PIXEL*)malloc(columnas* sizeof(PIXEL));

        if(*(matriz+i) == NULL)
        {
            liberarMatriz(matriz,i);
            return NULL;
        }
    }

    return matriz;

}
void llenarMatriz(IMAGEN bit_map, FILE *foto)
{
    size_t i, j,k, padding, fila_bytes;
    uint8_t bytes;
    fila_bytes = 3 * bit_map.cab_info.ancho;
    padding = (4 - (fila_bytes % 4)) % 4;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;


    for(i = 0 ; i < filas; i++)
    {

        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {

            for(k = 0 ; k < 3 ; k++)
            {

                fread(&bytes,1,1,foto);
                bit_map.pixeles[i][j].bgr[k] = bytes;

            }

        }
        fseek(foto, padding, SEEK_CUR);
    }




}
void mostrarMatriz(IMAGEN bit_map)
{
    size_t i, j, k;

    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                printf("%02x\t",bit_map.pixeles[i][j].bgr[k]);
            }
        }
        printf("\n");
    }
}
//////////////////////////////////////////CREAR IMAGEN//////////////////////////////////////////////////////
void CrearImagen(IMAGEN bit_map,char nombre[50])
{
    uint8_t byte,cero=0;
    size_t i,j,k,padding;
    FILE *nuevaimagen;
    padding = (4-(3*bit_map.cab_info.ancho)% 4)%4;


    nuevaimagen=fopen(nombre,"wb");
    if (!nuevaimagen)
    {
        perror("erro al crear imagen");
        exit(1);
    }
    fwrite(&bit_map.cab_file,sizeof(BMPFileHeader),1,nuevaimagen);

    fwrite(&bit_map.cab_info,sizeof(BMPInfoHeader),1,nuevaimagen);

    if(bit_map.cab_info.tamCabecera>40)
    {
        fwrite(&bit_map.vect,sizeof(uint8_t),bit_map.cab_info.tamCabecera-40,nuevaimagen);
    }
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;


    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                byte=bit_map.pixeles[i][j].bgr[k];
                fwrite(&byte,1,1,nuevaimagen);
            }
        }
        for(k=0; k<padding; k++)
        {
            fwrite(&cero,sizeof(cero),1,nuevaimagen);
        }
    }
    printf("imagen creada");
    fclose(nuevaimagen);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int extraerNumeroDesdeIgual( char *str, int *out)
{
    char *igual = strchr(str, '=');
    if (!igual || *(igual + 1) == '\0') {
        return 0;  // No hay '=' o nada después de él
    }

    char *numero = igual + 1;
    size_t len = strlen(numero);

    if (len == 0 || len > 4) return 0; // Máximo: signo + 3 cifras

    // Validación estricta: solo dígitos, con '-' solo al principio
    if (numero[0] == '-') {
        if (len == 1) return 0;  // Solo '-' no es válido
        for (int i = 1; numero[i]; i++) {
            if (!isdigit(numero[i])) return 0;
        }
    } else {
        for (int i = 0; numero[i]; i++) {
            if (!isdigit(numero[i])) return 0;
        }
    }

    // Validar longitud de dígitos (sin contar el signo)
    int cant_digitos = (numero[0] == '-') ? len - 1 : len;
    if (cant_digitos > 3) return 0;

    // Convertir
    *out = atoi(numero);
    return 1;
}
/////////////////////////////////////////NEGATIVO//////////////////////////////////////////////////////////////////////
void Negativo(IMAGEN bit_map,char nombre[50])
{
    size_t i, j, k;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {

                bit_map.pixeles[i][j].bgr[k]=255-bit_map.pixeles[i][j].bgr[k];

            }
        }

    }

    CrearImagen(bit_map,nombre);

}

//////////////////////////////////////////ESCALA DE GRISES///////////////////////////////////////////////
void EscaladeGrises(IMAGEN bit_map,char nombre[50])
{
    size_t i, j, k,aux=0;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;



    for(i = 0 ; i < filas ; i++)
    {

        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {


            for(k = 0 ; k < 3 ; k++)
            {
                aux = aux+bit_map.pixeles[i][j].bgr[k];
            }
            aux=aux/3;

            for(k = 0 ; k < 3 ; k++)
            {
                bit_map.pixeles[i][j].bgr[k]=aux;
            }
            aux=0;
        }

    }
    CrearImagen(bit_map,nombre);
}

///////////////////////////////////////ESPEJAR HORIZONTAL////////////////////////////////////////////////
void EspejarHorizontal(IMAGEN bit_map,char nombre[50])
{
    size_t i, j, k, fin_col;
    PIXEL aux;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;



    for(i = 0 ; i <filas ; i++)
    {
        fin_col=bit_map.cab_info.ancho-1;
        for(j = 0 ; j < fin_col ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                aux.bgr[k]=bit_map.pixeles[i][j].bgr[k];

                bit_map.pixeles[i][j].bgr[k]=bit_map.pixeles[i][fin_col].bgr[k];

                bit_map.pixeles[i][fin_col].bgr[k]=aux.bgr[k];
            }
            fin_col--;

        }

    }
    CrearImagen(bit_map,nombre);
}

//////////////////////////////////ESPEJADO VERTICAL//////////////////////////////////////////////////
void EspejarVertical(IMAGEN bit_map,char nombre[50])
{
    size_t i, j, k, fin_fi;
    PIXEL aux;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;

    for(i = 0 ; i <bit_map.cab_info.ancho ; i++)
    {
        fin_fi=filas-1;
        for(j = 0 ; j < fin_fi ; j++)
        {
            for(k = 0 ; k < 3 ; k++)
            {
                aux.bgr[k]=bit_map.pixeles[j][i].bgr[k];

                bit_map.pixeles[j][i].bgr[k]=bit_map.pixeles[fin_fi][i].bgr[k];

                bit_map.pixeles[fin_fi][i].bgr[k]=aux.bgr[k];
            }
            fin_fi--;

        }
    }
    CrearImagen(bit_map,nombre);
}

/////////////////////////////////AUMENTAR CONTRASTE/////////////////////////////////
void AumentarContraste(IMAGEN bit_map,char nombre[50],float porcentaje)
{

    size_t i, j, k;
    float nuevo_valor=0;
    float factor = (porcentaje < 0) ? 0 : (porcentaje>100)? 100:porcentaje;
    factor=factor/100.0+1.0;

    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;

    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {

            for(k = 0 ; k < 3 ; k++)
            {
                nuevo_valor =128.0+(factor*((float)bit_map.pixeles[i][j].bgr[k]-128.0));
                if (nuevo_valor <= 0)
                {
                    nuevo_valor = 0;
                }
                if (nuevo_valor > 255)
                {
                    nuevo_valor = 255;
                }
                bit_map.pixeles[i][j].bgr[k] = nuevo_valor;
            }

        }
    }
    CrearImagen(bit_map,nombre);
}

/////////////////////////////REDUCIR CONTRASTE////////////////////////////////////////
void ReducirContraste(IMAGEN bit_map,char nombre[50],float porcentaje)
{

    size_t i, j, k;
    float nuevo_valor=0;
    float factor = (porcentaje < 0) ? 0 : (porcentaje>100)? 100:porcentaje;
    factor =factor/100.0;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    for(i = 0 ; i < filas ; i++)
    {
        for(j = 0 ; j < bit_map.cab_info.ancho ; j++)
        {

            for(k = 0 ; k < 3 ; k++)
            {
                nuevo_valor =128.0+(1.0-factor)*((float)bit_map.pixeles[i][j].bgr[k]-128.0);
                if (nuevo_valor <= 0)
                {
                    nuevo_valor = 0;
                }
                if (nuevo_valor > 255)
                {
                    nuevo_valor = 255;
                }
                bit_map.pixeles[i][j].bgr[k] = nuevo_valor;
            }

        }

    }
    CrearImagen(bit_map,nombre);

}
/////////////////////////////////////////////////////TONALIDAD AZUL//////////////////////////////////
void TonalidadAzul(IMAGEN bit_map,char nombre[50],float porcentaje)
{
    float factor = (porcentaje < -100) ? -100 : (porcentaje>100)? 100:porcentaje;
    factor = 1.0 + (porcentaje / 100.0);
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;

    for (size_t i = 0; i < filas; i++)
    {
        for (size_t j = 0; j < columnas; j++)
        {
            float azul_modificado = (float)bit_map.pixeles[i][j].bgr[0] * factor;

            if (azul_modificado > 255.0)
                bit_map.pixeles[i][j].bgr[0] = 255;
            else if (azul_modificado < 0.0)
                bit_map.pixeles[i][j].bgr[0] = 0;
            else
                bit_map.pixeles[i][j].bgr[0] = (uint8_t)azul_modificado;
        }
    }
    CrearImagen(bit_map,nombre);
}
///////////////////////////////TONALIDAD VERDE////////////////////////////
void TonalidadVerde(IMAGEN bit_map,char nombre[50],float porcentaje)
{
    float factor = (porcentaje < -100) ? -100 : (porcentaje>100)? 100:porcentaje;
    factor = 1.0 + (porcentaje / 100.0);
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;

    for (size_t i = 0; i < filas; i++)
    {
        for (size_t j = 0; j < columnas; j++)
        {
            float verde_modificado = (float)bit_map.pixeles[i][j].bgr[1] * factor;

            if (verde_modificado > 255.0)
                bit_map.pixeles[i][j].bgr[1] = 255;
            else if (verde_modificado < 0.0)
                bit_map.pixeles[i][j].bgr[1] = 0;
            else
                bit_map.pixeles[i][j].bgr[1] = (uint8_t)verde_modificado;
        }
    }
    CrearImagen(bit_map,nombre);
}

/////////////////////////////////////TONALIDAD ROJO////////////////////////////
void TonalidadRoja(IMAGEN bit_map,char nombre[50],float porcentaje)
{
    float factor = (porcentaje < -100) ? -100 : (porcentaje>100)? 100:porcentaje;
    factor = 1.0 + (porcentaje / 100.0);
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;

    for (size_t i = 0; i < filas; i++)
    {
        for (size_t j = 0; j < columnas; j++)
        {
            float rojo_modificado = (float)bit_map.pixeles[i][j].bgr[2] * factor;

            if (rojo_modificado > 255.0)
                bit_map.pixeles[i][j].bgr[2] = 255;
            else if (rojo_modificado < 0.0)
                bit_map.pixeles[i][j].bgr[2] = 0;
            else
                bit_map.pixeles[i][j].bgr[2] = (uint8_t)rojo_modificado;
        }
    }

}
////////////////////////////////////////RECORTAR//////////////////////////////////////////
void recortarImagen(IMAGEN bit_map, char nombre[50], float porcentaje)
{
    float factor = (porcentaje < 0) ? 0 : (porcentaje>100)? 100:porcentaje;
    factor = 1.0 - (porcentaje / 100.0);
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;
    int fila_redu = (filas * factor);
    int columna_redu = (columnas * factor);
    int padding;

    PIXEL **recorte = crearMatriz(fila_redu,columna_redu);
    if (!recorte)
    {
        printf("error al crear nueva matriz recortada");
        exit(1);
    }

    for (size_t i = 0; i < fila_redu; i++)
    {
        for (size_t j = 0; j < columna_redu; j++)
        {
            recorte[i][j] = bit_map.pixeles[i][j];
        }
    }
    padding=(4-(columna_redu*3)%4)%4;
    bit_map.cab_info.tamImagen=(columna_redu*3+padding)*fila_redu;
    bit_map.cab_file.tamArchivo=bit_map.cab_info.tamImagen+bit_map.cab_info.tamCabecera+sizeof(uint8_t)*14;
    bit_map.cab_info.ancho=columna_redu;
    bit_map.cab_info.alto=bit_map.cab_info.alto*(factor);
    bit_map.pixeles=recorte;
    CrearImagen(bit_map,nombre);
    liberarMatriz(recorte,fila_redu);
}


////////////////////////////////////////ACHICAR////////////////////////////////////////////////
void AchicarImagen(IMAGEN bit_map,char nombre[50], float factor)
{
    PIXEL **matriz_reducida;
    int ancho_redu, alto_redu, x, y, x1, y1,i,j,k,columnas;
    size_t padding;
    float escala_x, escala_y;
    if (factor <= 0.0 || factor > 100.0)
        exit(1);

    factor=factor/100.0;

    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    columnas=bit_map.cab_info.ancho;
    alto_redu=filas*(1-factor);

    ancho_redu=bit_map.cab_info.ancho*(1-factor);

    escala_x=bit_map.cab_info.ancho/ancho_redu;
    escala_y=filas/alto_redu;


    padding=(4-(ancho_redu*3)%4)%4;
    bit_map.cab_info.tamImagen=(ancho_redu*3+padding)*alto_redu;
    bit_map.cab_file.tamArchivo=bit_map.cab_info.tamImagen+bit_map.cab_info.tamCabecera+sizeof(uint8_t)*14;
    bit_map.cab_info.ancho=ancho_redu;
    bit_map.cab_info.alto=bit_map.cab_info.alto*(1-factor);

    matriz_reducida=crearMatriz(alto_redu,ancho_redu);
    if (!matriz_reducida)
    {
        printf("error al crear nueva matriz reducida");
        exit(1);
    }



    for(i = 0 ; i < alto_redu ; i++)
    {
        y = (int)escala_y * i;
        y1 = (y+1<filas)?y+1:filas-1;

        for(j = 0 ; j < ancho_redu ; j++)
        {
            x = (int)escala_x * j;
            x1 = (x+1<columnas)?x+1:columnas-1;

            for(k = 0 ; k < 3 ; k++)
            {

                matriz_reducida[i][j].bgr[k]=(bit_map.pixeles[y][x].bgr[k] + bit_map.pixeles[y][x1].bgr[k] + bit_map.pixeles[y1][x].bgr[k] + bit_map.pixeles[y1][x1].bgr[k]) / 4;

            }

        }

    }

    bit_map.pixeles=matriz_reducida;

    CrearImagen(bit_map,nombre);
    liberarMatriz(matriz_reducida,alto_redu);
}

////////////////////////////////ROTAR DERECHA////////////////////////////////
void RotarDerecha(IMAGEN bit_map,char nombre[50])
{

    int i, j;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;
    int aux;
    PIXEL** nueva_matriz = crearMatriz(columnas, filas);
    if (!nueva_matriz)
    {
        printf("error al crear nueva matriz de rotacion");
        exit(1);
    }


    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < columnas; j++)
        {
            nueva_matriz[j][i] = bit_map.pixeles[i][j];
        }
    }
    bit_map.pixeles=nueva_matriz;
    aux=bit_map.cab_info.alto;
    bit_map.cab_info.alto=bit_map.cab_info.ancho;
    bit_map.cab_info.ancho=aux;

    CrearImagen(bit_map,nombre);

}

////////////////////////////////ROTAR IZQUIERDA////////////////////////////////
void RotarIzquierda(IMAGEN bit_map, char nombre[50])
{

    int i, j;
    int filas = (bit_map.cab_info.alto > 0) ? bit_map.cab_info.alto : -bit_map.cab_info.alto;
    int columnas = bit_map.cab_info.ancho;
    int aux;
    PIXEL** nueva_matriz = crearMatriz(columnas, filas);
    if (!nueva_matriz)
    {
        printf("error al crear nueva matriz de rotacion");
        exit(1);
    }

    for (i = 0; i < filas; i++)
    {
        for (j = 0; j < columnas; j++)
        {
            nueva_matriz[j][filas - i - 1] = bit_map.pixeles[i][j];
        }
    }
    bit_map.pixeles=nueva_matriz;
    aux=bit_map.cab_info.alto;
    bit_map.cab_info.alto=bit_map.cab_info.ancho;
    bit_map.cab_info.ancho=aux;

    CrearImagen(bit_map,nombre);

}
////////////////////////////////////////////////////////////////
void invertirVertical(PIXEL **matriz, size_t filas) {
    for (size_t i = 0; i < filas / 2; i++) {
        PIXEL *temp = matriz[i];
        matriz[i] = matriz[filas - 1 - i];
        matriz[filas - 1 - i] = temp;
    }
}

/////////////////////////////CONCATENAR HORIZONTAL///////////////////////////
void ConcatenarHorizontal(IMAGEN bit_map, IMAGEN bit_map_2, char nombre[40])
{
    // Obtener dimensiones reales
    size_t filas1 = (bit_map.cab_info.alto < 0) ? -bit_map.cab_info.alto : bit_map.cab_info.alto;
    size_t filas2 = (bit_map_2.cab_info.alto < 0) ? -bit_map_2.cab_info.alto : bit_map_2.cab_info.alto;
    size_t columnas1 = bit_map.cab_info.ancho;
    size_t columnas2 = bit_map_2.cab_info.ancho;

    bool img1_arriba = bit_map.cab_info.alto < 0;
    bool img2_arriba = bit_map_2.cab_info.alto < 0;

    if (img1_arriba != img2_arriba)
    {
        if (img1_arriba)
            invertirVertical(bit_map.pixeles, filas1);
        else
            invertirVertical(bit_map_2.pixeles, filas2);
    }

    size_t maxFilas;

    if(filas1 > filas2)
    {

        maxFilas = filas1;
        bit_map.cab_info.alto = bit_map.cab_info.alto;
    }
    else
    {
        maxFilas = filas2;
        bit_map.cab_info.alto = bit_map_2.cab_info.alto;
    }
    size_t nuevasColumnas = columnas1 + columnas2;


    // Crear nueva matriz
    PIXEL **nuevaMatriz = crearMatriz(maxFilas, nuevasColumnas);
    if (!nuevaMatriz)
    {
        printf("error al crear nueva matriz extendida");
        exit(1);
    }

    // Color de relleno (Rosa chicle?)
    PIXEL relleno;
    relleno.bgr[0] = 180;
    relleno.bgr[1] = 105;
    relleno.bgr[2] = 255;

    // Copiar imagen 1
    for (size_t i = 0; i < maxFilas; i++)
    {
        for (size_t j = 0; j < columnas1; j++)
        {
            if (i < filas1)
                nuevaMatriz[i][j] = bit_map.pixeles[i][j];
            else
                nuevaMatriz[i][j] = relleno;
        }
    }

    // Copiar imagen 2
    for (size_t i = 0; i < maxFilas; i++)
    {
        for (size_t j = 0; j < columnas2; j++)
        {
            if (i < filas2)
                nuevaMatriz[i][j + columnas1] = bit_map_2.pixeles[i][j];
            else
                nuevaMatriz[i][j + columnas1] = relleno;
        }
    }

    // Construir los nuevos headers

    bit_map.pixeles=nuevaMatriz;
    bit_map.cab_info.ancho = nuevasColumnas;

    size_t fila_bytes = nuevasColumnas * 3;
    size_t padding = (4 - (fila_bytes % 4)) % 4;
    bit_map.cab_info.tamImagen = (fila_bytes + padding) * maxFilas;

    bit_map.cab_file.tamArchivo = bit_map.cab_info.tamImagen + bit_map.cab_info.tamCabecera + sizeof(uint8_t)*14;

    // Llamar a CrearImagen
    CrearImagen(bit_map,nombre);  // Usa vect extensible original
    liberarMatriz(nuevaMatriz, maxFilas);
}
//////////////////////////////////////CONCATENAR VERTICAL///////////////////////////////////////
void ConcatenarVertical(IMAGEN bit_map, IMAGEN bit_map_2, char nombre[40])
{
    // Obtener dimensiones reales (altura positiva)
    size_t filas1 = (bit_map.cab_info.alto < 0) ? -bit_map.cab_info.alto : bit_map.cab_info.alto;
    size_t filas2 = (bit_map_2.cab_info.alto < 0) ? -bit_map_2.cab_info.alto : bit_map_2.cab_info.alto;
    size_t columnas1 = bit_map.cab_info.ancho;
    size_t columnas2 = bit_map_2.cab_info.ancho;

    size_t nuevasFilas = filas1 + filas2;
    size_t maxColumnas = (columnas1 > columnas2) ? columnas1 : columnas2;

    bool img1_arriba = bit_map.cab_info.alto < 0;
    bool img2_arriba = bit_map_2.cab_info.alto < 0;



    if (img1_arriba != img2_arriba)
    {
        if (img1_arriba)
            invertirVertical(bit_map.pixeles, filas1);

        else
            invertirVertical(bit_map_2.pixeles, filas2);

        bit_map.cab_info.alto = nuevasFilas;

    }
    else
    {
        if(img1_arriba)
            bit_map.cab_info.alto = -nuevasFilas;
        else
            bit_map.cab_info.alto = nuevasFilas;
    }
    // Crear nueva matriz
    PIXEL** nuevaMatriz = crearMatriz(nuevasFilas, maxColumnas);
    if (!nuevaMatriz)
    {
        printf("error al crear nueva matriz extendida");
        exit(1);
    }

    // Color de relleno (verde oscuro por ejemplo)
    PIXEL relleno;
    relleno.bgr[0] = 180;
    relleno.bgr[1] = 105;
    relleno.bgr[2] = 255;
 // BGR: verde oscuro

    // Copiar imagen1 arriba
    for (size_t i = 0; i < filas1; i++)
    {
        for (size_t j = 0; j < maxColumnas; j++)
        {
            if (j < columnas1)
                nuevaMatriz[i][j] = bit_map.pixeles[i][j];
            else
                nuevaMatriz[i][j] = relleno;
        }
    }

    // Copiar imagen2 abajo
    for (size_t i = 0; i < filas2; i++)
    {
        for (size_t j = 0; j < maxColumnas; j++)
        {
            if (j < columnas2)
                nuevaMatriz[i + filas1][j] = bit_map_2.pixeles[i][j];
            else
                nuevaMatriz[i + filas1][j] = relleno;
        }
    }


    bit_map.pixeles=nuevaMatriz;

    bit_map.cab_info.ancho = maxColumnas;

    size_t fila_bytes = maxColumnas * 3;
    size_t padding = (4 - (fila_bytes % 4)) % 4;
    bit_map.cab_info.tamImagen = (fila_bytes + padding) * nuevasFilas;

    bit_map.cab_file.tamArchivo = bit_map.cab_info.tamImagen + bit_map.cab_info.tamCabecera + sizeof(uint8_t)*14;

    // Crear imagen
    CrearImagen(bit_map,nombre);

    // Liberar memoria
    liberarMatriz(nuevaMatriz, nuevasFilas);
}
///////////////////////////////////////////////////////////////////////////////
void Comodin()
{
    printf("puta");
}


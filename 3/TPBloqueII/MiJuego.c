//authors José María Belando López y Francisco Muñoz Martínez

#include "MiJuego.h"
#include "MiSDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define HMIN 15
#define TAMNOMBRE 20
#define TAMUSUARIOS 5
#include "MiArbol.h"

typedef struct
    {
        char nombre[TAMNOMBRE];
        int tiempo;
    } Usuario;

typedef struct snakeCelda
{
    int coordenadaX;
    int coordenadaY;
    struct snakeCelda* cuadradoSIG;
} snakeCeldaRep;
typedef snakeCeldaRep* snakeCeldaAp;

typedef struct
{
    MiSDL  miSDL;
    snakeCeldaAp serpientePrimera,serpienteUltima;
    int ancho, alto;
    SDL_Rect cuadrado;
    SDL_Rect cuadradoAleatorio;
    SDL_Color  colorCuadradoBorde, colorCuadradoRelleno;
    SDL_Rect tuberia;
    SDL_Color colorTuberiaBorde, colorTuberiaRelleno;
    int puntuacion;
    int longitudSerpiente;
    int tiempoFase; //Sirve para llevar el tiempo de transicion entre las fases 5 y 7 en la función anterior.
    int tiempoEspera; //Contiene el tiempo que tiene que esperar. La pausa definida por el usuario. Se compara con tiempoFase.
    int puntuacionGanadora; //Contiene los puntos a partir de los cual se iniciará el bonus del juego.
    int superRetardoMin; //Contiene el retardo mínimo en el bonus.
    int retardoNormal; //Sirve para no perder el retardo al modificarlo en el bonus.
    int longitudSerpienteAnterior; // Para no perder la longitud en el bonus.
    int dx,dy;
    int retardo;
    int retardoMin; //Es el retardo mínimo para la serpiente.
    int fase;
    int tiempo;
    char nombre[TAMNOMBRE];
    Usuario usuarios[TAMUSUARIOS];
    TTF_Font * fuente1, * fuente2, * fuente3;
    SDL_Color colorTexto1,colorRectanguloBorde1,colorRectanguloRelleno1;
    SDL_Color colorTexto2,colorRectanguloBorde2,colorRectanguloRelleno2;
    SDL_Color colorTexto3,colorRectanguloBorde3,colorRectanguloRelleno3;
    char nombreFichero[40];
    int nPasos, nPasosMax; //El número de pasos para aumentar el retardo en el bonus
    int puntuacionAnterior; //Guarda la puntuación cuando sale del bonus
    SDL_Color colorTextoMarcador, colorRectanguloBordeMarcador, colorRectanguloRellenoMarcador;
    SDL_Color colorRectanguloBordeEspecial;
    SDL_Color colorRectanguloRellenoEspecial; //Es el rectángulo de la versión bonus
    int faseAnterior;
    int pausa;
} MiJuegoRep;
typedef MiJuegoRep * MiJuegoAp;

/**
AMPLIACIÓN
*/
int MiSnake_CompararCabeza(MiJuegoAp miJuego)
{
    if ((miJuego->serpientePrimera->coordenadaX==miJuego->cuadradoAleatorio.x) && (miJuego->serpientePrimera->coordenadaY==miJuego->cuadradoAleatorio.y)) return 1;
    return 0;
    //Devuelve 0 si coordenada X y coordenda Y coinciden con los de la serpiente
}

int miSnake_CompararTuberia(MiJuegoAp miJuego)
{
    if ((miJuego->serpientePrimera->coordenadaX==miJuego->tuberia.x) && (miJuego->serpientePrimera->coordenadaY==miJuego->tuberia.y)) return 1;
    return 0;
}

void MiJuego_Error(char * error)
    {
        printf("%s",error);
        exit(EXIT_FAILURE);
    }

/**
AMPLIACIÓN
*/
SDL_Rect MiJuego_generarRectanguloAleatorio( int largo, int ancho, int lado)
{
     SDL_Rect cuadrado;
     cuadrado.h=lado;
     cuadrado.w =lado;
/*
            El razonamiento es el siguiente:
              Para que la coordenada coincida con una posición exacta en la
              que tiene que estar el cuadrado, hay que ver cuáles son el nú-
              mero de cuadros posibles dentro del eje.
              n = ancho/lado; largo/lado
              Ahora, la coordenada a pintar vendrá dada por:
              coordenada = 0 + i*lado
              Y el número i vendrá generado aleatoriamente entre 0 y n-1 que
              es el número de valores posibles sin salirse de la cuadrícula
*/
    int n = largo / lado;
    int a = 0;
    int b=n-1;
    int i = a+rand()%(b-a+1);
    cuadrado.y = 0 + i*lado;
    // Ahora para el ancho o eje x
    n = ancho / lado;
    // a = 0
    b = n-1;
    i = a+rand()%(b-a+1);
    cuadrado.x = 0 + i*lado;
    return cuadrado;
}

int MiJuego_Chocar(MiJuegoAp miJuego)
{
    int x = miJuego->serpientePrimera->coordenadaX;
    int y = miJuego->serpientePrimera->coordenadaY;
    if((x < 0) || (y < 0) || (x+miJuego->cuadrado.w > miJuego->ancho) || (y + miJuego->cuadrado.h> miJuego->alto)) return 1;
    snakeCeldaAp aux=miJuego->serpienteUltima;
    while((aux->cuadradoSIG!=NULL) && ((x!=aux->coordenadaX) || (y!=aux->coordenadaY)))
    {
            aux = aux->cuadradoSIG;
    }
    return (aux->cuadradoSIG!=NULL);
}

void MiJuego_IniciarFase(MiJuegoAp miJuego, int fase)
{
    miJuego->fase = fase;
    switch(fase)
    {
                case 0:
                    miJuego->longitudSerpiente = 0;
                    miJuego->puntuacion=0;
                    miJuego->serpientePrimera=malloc(sizeof(snakeCeldaRep));
                    miJuego->serpientePrimera->coordenadaX=0;
                    miJuego->serpientePrimera->coordenadaY=0;
                    miJuego->cuadrado.x=0;
                    miJuego->cuadrado.y =0;
                    miJuego->serpientePrimera->cuadradoSIG=NULL;
                    miJuego->serpienteUltima = miJuego->serpientePrimera;
                    break;
                 case 1:
                    miJuego->cuadradoAleatorio= MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho,miJuego->cuadrado.w);
                    miJuego->tiempo = time(NULL);
                    miJuego->nPasos = 0;
                    miJuego->puntuacionAnterior = 0;
                    break;
                 case 2:
                    strcpy(miJuego->nombre,"");
                    SDL_StartTextInput();
                    break;

                /**
				AMPLIACIÓN
				*/
                 case 3:
                     miJuego->cuadradoAleatorio = MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho,miJuego->cuadrado.w);
                     break;
                 case 4:
                     miJuego->longitudSerpienteAnterior = miJuego->longitudSerpiente;
                     break;
                 case 5:
                     miJuego->tiempoFase = time(NULL); //Se coge el tiempo de espera de la fase
                     break;
                 case 6:
                     miJuego->serpientePrimera=malloc(sizeof(snakeCeldaRep));
                     miJuego->serpientePrimera->coordenadaX=0;
                     miJuego->serpientePrimera->coordenadaY=0;
                     miJuego->serpientePrimera->cuadradoSIG=NULL;
                     miJuego->serpienteUltima = miJuego->serpientePrimera;
                     miJuego->tiempo = time(NULL);
                     miJuego->nPasos = 0;
                     miJuego->cuadradoAleatorio = MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho,miJuego->cuadrado.w);
                     miJuego->retardoNormal = miJuego->retardo; //Lo hace para que al volver a la fase se quede guardado el valor
                     miJuego->retardo = 200; //Ahora la serpiente ira mucho mas rapida
                     miJuego->dx=1;
                     miJuego->dy=0;
                     break;
                 case 7:
                     miJuego->serpientePrimera=malloc(sizeof(snakeCeldaRep));
                     miJuego->serpientePrimera->coordenadaX=miJuego->tuberia.x;
                     miJuego->serpientePrimera->coordenadaY=miJuego->tuberia.y;
                     miJuego->serpientePrimera->cuadradoSIG=NULL;
                     miJuego->serpienteUltima = miJuego->serpientePrimera;
                     miJuego->tiempoFase = time(NULL);
                     break;
                  case 8:
                     miJuego->dx = 0;
                     miJuego->dy = 1;
                     miJuego->cuadradoAleatorio = MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho,miJuego->cuadrado.w);
                     break;
                  case 9:
                     break;
    }
}

void MiJuego_TerminarFase(MiJuegoAp miJuego)
{
     switch(miJuego->fase)
     {
        case 0: MiJuego_IniciarFase(miJuego,1);  break;
        case 1:
            if (MiJuego_Chocar(miJuego)) {

            while(miJuego->serpienteUltima!=NULL)
                {
                    snakeCeldaAp aux = miJuego->serpienteUltima;
                    miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
                    free(aux);
                }
                miJuego->tiempo = time(NULL) - miJuego->tiempo;
               if (miJuego->puntuacion > miJuego->usuarios[TAMUSUARIOS-1].tiempo)
                   MiJuego_IniciarFase(miJuego,2);
               else MiJuego_IniciarFase(miJuego,0);
            }
            else
               MiJuego_IniciarFase(miJuego,3);
               break;
        case 2:
               SDL_StopTextInput();
               int i=TAMUSUARIOS-1;
               while ((i>0)&&(miJuego->puntuacion > miJuego->usuarios[i-1].tiempo) && (i>=0))
               {
                    strcpy(miJuego->usuarios[i].nombre,miJuego->usuarios[i-1].nombre);
                    miJuego->usuarios[i].tiempo = miJuego->usuarios[i-1].tiempo;
                    i--;
               }
               strcpy(miJuego->usuarios[i].nombre,miJuego->nombre);
               miJuego->usuarios[i].tiempo = miJuego->puntuacion;
               FILE * ficheroRecord;
               ficheroRecord = fopen(miJuego->nombreFichero,"w");
               for(int i=0;i<TAMUSUARIOS;i++) fprintf(ficheroRecord,"%s %d", miJuego->usuarios[i].nombre,miJuego->usuarios[i].tiempo);
               fclose(ficheroRecord);
               MiJuego_IniciarFase(miJuego,0);
               break;

               /**
				AMPLIACIÓN
				*/
        case 3: //La tuberia esta pintada pero aun no ha chocado con ella la serpiente
               if (MiJuego_Chocar(miJuego)) {

               while(miJuego->serpienteUltima!=NULL)
               {
                    snakeCeldaAp aux = miJuego->serpienteUltima;
                    miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
                    free(aux);
               }
               miJuego->tiempo = time(NULL) - miJuego->tiempo;
                  if (miJuego->puntuacion > miJuego->usuarios[TAMUSUARIOS-1].tiempo)
                   MiJuego_IniciarFase(miJuego,2);
                  else MiJuego_IniciarFase(miJuego,0);
               }
               else
               MiJuego_IniciarFase(miJuego, 4);
               break;
        case 4: //Es la fase en la que se mete la serpiente por la tuberia
               MiJuego_IniciarFase(miJuego, 5);
               break;
        case 5:
               MiJuego_IniciarFase(miJuego, 6);
               break;
        case 6:
               while(miJuego->serpienteUltima!=NULL)
               {
                    snakeCeldaAp aux = miJuego->serpienteUltima;
                    miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
                    free(aux);
               }
               miJuego->retardo = miJuego->retardoNormal; //Se restaura el valor de retardo
               MiJuego_IniciarFase(miJuego, 7);
               break;
        case 7:
               MiJuego_IniciarFase(miJuego, 8);
               break;
        case 8:
               if (MiJuego_Chocar(miJuego)) {

               while(miJuego->serpienteUltima!=NULL)
               {
                    snakeCeldaAp aux = miJuego->serpienteUltima;
                    miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
                    free(aux);
               }
               miJuego->tiempo = time(NULL) - miJuego->tiempo;
                  if (miJuego->puntuacion > miJuego->usuarios[TAMUSUARIOS-1].tiempo)
                      MiJuego_IniciarFase(miJuego,2);
                  else MiJuego_IniciarFase(miJuego,0);
               }
               else {
                   miJuego->longitudSerpiente = miJuego->longitudSerpienteAnterior;
                   miJuego->dx=0;
                   miJuego->dy=1;
                   MiJuego_IniciarFase(miJuego, 1);
               }
               break;
        case 9:
               miJuego->fase = miJuego->faseAnterior;
               break;
     }
}

/**
AMPLIACIÓN
*/
void  MiJuego_TiempoEspera(MiJuegoAp miJuego)
{
    int intervaloTiempo = time(NULL) - miJuego->tiempoFase;
    if (intervaloTiempo>=(miJuego->tiempoEspera)) MiJuego_TerminarFase(miJuego);
}

void MiJuego_CambiarDireccion(MiJuegoAp miJuego, SDL_Keycode code)
{
     switch(code){
           case SDLK_LEFT: miJuego->dx=-1;
                           miJuego->dy=0;
                           break;
            case SDLK_RIGHT:
                           miJuego->dx=1;
                           miJuego->dy=0;
                           break;
            case SDLK_UP:
                           miJuego->dx=0;
                           miJuego->dy=-1;
                           break;
            case SDLK_DOWN:
                           miJuego->dx=0;
                           miJuego->dy=1;
                           break;
            case SDLK_RETURN:
                           if(miJuego->pausa==0 ){
                           miJuego->pausa = 1;
                           miJuego->faseAnterior = miJuego->fase;
                           MiJuego_IniciarFase(miJuego, 9);
                           }
                           break;
             }
}

void MiJuego_EventoTeclado(MiJuegoAp miJuego, SDL_Keycode code)
{
    switch(miJuego->fase){
        case 0: if ((code==SDLK_LEFT) || (code==SDLK_RIGHT) || (code==SDLK_UP) ||(code==SDLK_DOWN))
                    MiJuego_TerminarFase(miJuego);
        case 1:
            MiJuego_CambiarDireccion(miJuego, code);
            break;
        case 2:
            if ((code==SDLK_RETURN)&&(strlen(miJuego->nombre)>0)) MiJuego_TerminarFase(miJuego);
            if ((code==SDLK_BACKSPACE)&&(strlen(miJuego->nombre)>0)) miJuego->nombre[strlen(miJuego->nombre)-1] = '\0';
            break;
        case 3:
            MiJuego_CambiarDireccion(miJuego, code);
            break;
        case 6:
            MiJuego_CambiarDireccion(miJuego, code);
            break;
        case 8:
             MiJuego_CambiarDireccion(miJuego, code);
             break;
         case 9:
              switch(code)
                   {
                   case SDLK_RETURN:
                       if (miJuego->pausa==1)
                                {
                                  miJuego->pausa=0;
                                  MiJuego_TerminarFase(miJuego);
                                }
                    break;
                    }
              break;
    }
}

void MiJuego_Mover(MiJuegoAp miJuego)
{
    if ((miJuego->fase==1) || (miJuego->fase==3))
    {
       snakeCeldaAp aux = malloc(sizeof(snakeCeldaRep));
       aux->coordenadaX = miJuego->serpientePrimera->coordenadaX + miJuego->dx*miJuego->cuadrado.w;
       aux->coordenadaY = miJuego->serpientePrimera->coordenadaY + miJuego->dy*miJuego->cuadrado.h;
       aux->cuadradoSIG = NULL;
       miJuego->serpientePrimera->cuadradoSIG = aux;
       miJuego->serpientePrimera = aux;

    if(MiSnake_CompararCabeza(miJuego))
    {
       miJuego->cuadradoAleatorio=MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho, miJuego->cuadrado.w); //poner parametros
       if ((miJuego->retardo)>(miJuego->retardoMin)) miJuego->retardo--;
       miJuego->puntuacion++;
       miJuego->puntuacionAnterior++;
       miJuego->longitudSerpiente++;
    }
    else
    {
       snakeCeldaAp aux = miJuego->serpienteUltima;
       miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
       free(aux);
    }
    int choque = MiJuego_Chocar(miJuego);
    if(choque) MiJuego_TerminarFase(miJuego);
    switch(miJuego->fase) {
    case 1:
        if (miJuego->puntuacionAnterior >= miJuego->puntuacionGanadora) MiJuego_TerminarFase(miJuego);
        break;
    case 3:
        if(miSnake_CompararTuberia(miJuego)) MiJuego_TerminarFase(miJuego);
        break;
    }
    }
    if (miJuego->fase==4)    //El último lo libera
    {
        if(miJuego->serpienteUltima==miJuego->serpientePrimera) MiJuego_TerminarFase(miJuego);
        snakeCeldaAp aux = miJuego->serpienteUltima;
        miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
        free(aux);
    }
    if (miJuego->fase==6) //Va comiendo rectángulos al mismo tiempo que la serpiente aumenta de tamano
    {
    snakeCeldaAp aux = malloc(sizeof(snakeCeldaRep));
    aux->coordenadaX = miJuego->serpientePrimera->coordenadaX + miJuego->dx*miJuego->cuadrado.w;
    aux->coordenadaY = miJuego->serpientePrimera->coordenadaY + miJuego->dy*miJuego->cuadrado.h;
    aux->cuadradoSIG = NULL;
    miJuego->serpientePrimera->cuadradoSIG = aux;
    miJuego->serpientePrimera = aux;
    miJuego->nPasos++;
    if (miJuego->nPasos==miJuego->nPasosMax)
    {
       miJuego->nPasos = 0;
       if (miJuego->retardo>=miJuego->superRetardoMin) miJuego->retardo=(miJuego->retardo) - 5;
    }
    else
    {
       snakeCeldaAp aux = miJuego->serpienteUltima;
       miJuego->serpienteUltima = miJuego->serpienteUltima->cuadradoSIG;
       free(aux);
    }
    if(MiSnake_CompararCabeza(miJuego))
    {
       miJuego->cuadradoAleatorio=MiJuego_generarRectanguloAleatorio(miJuego->alto, miJuego->ancho, miJuego->cuadrado.w); //poner parametros
       miJuego->puntuacion= (miJuego->puntuacion + 15);
    }
    int choque = MiJuego_Chocar(miJuego);
    if(choque) MiJuego_TerminarFase(miJuego);
    } //Fin del caso

    if (miJuego->fase==8)
    {
        if (miJuego->longitudSerpiente != 0)
        {
            snakeCeldaAp aux = malloc(sizeof(snakeCeldaRep));
            aux->coordenadaX = miJuego->serpientePrimera->coordenadaX + miJuego->dx*miJuego->cuadrado.w;
            aux->coordenadaY = miJuego->serpientePrimera->coordenadaY + miJuego->dy*miJuego->cuadrado.h;
            aux->cuadradoSIG = NULL;
            miJuego->serpientePrimera->cuadradoSIG = aux;
            miJuego->serpientePrimera = aux;
            miJuego->longitudSerpiente--;
            int choque = MiJuego_Chocar(miJuego);
            if(choque) MiJuego_TerminarFase(miJuego);
        }
        else
        MiJuego_TerminarFase(miJuego);
    }
}

void MiJuego_Pintar(MiJuegoAp miJuego)
{
    MiSDL_CopiarTextura(miJuego->miSDL);
    char puntua[10];
    switch (miJuego->fase)
    {
        case 0:
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                for (int i=0;i<TAMUSUARIOS;i++)
                {
                    char aux[100];
                    sprintf(aux,"%d : %s - %d .puntos",i+1,miJuego->usuarios[i].nombre,miJuego->usuarios[i].tiempo);
                    MiSDL_PintarTexto(miJuego->miSDL,100+i*50,aux,miJuego->colorTexto1,miJuego->colorRectanguloBorde1,miJuego->colorRectanguloRelleno1,miJuego->fuente1);
                }
                MiSDL_PintarTexto(miJuego->miSDL,200+(TAMUSUARIOS*50),"Pulse FLECHAS para JUGAR",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                break;
        case 1: //Vamos a pintar la serpiente por cada vuelta
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadradoAleatorio,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                }
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                break;
        case 2:
                {
                    char aux[100];
                    sprintf(aux, " Puntuacion=%d puntos" ,miJuego->puntuacion);
                    MiSDL_PintarTexto(miJuego->miSDL,250,aux,miJuego->colorTexto1,miJuego->colorRectanguloBorde1,miJuego->colorRectanguloRelleno1,miJuego->fuente1);
                    MiSDL_PintarTexto(miJuego->miSDL,300,"Introduce nombre:",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                    MiSDL_PintarTextoEntrada(miJuego->miSDL,350,400,60,miJuego->nombre,miJuego->colorTexto3,miJuego->colorRectanguloBorde3,miJuego->colorRectanguloRelleno3,miJuego->fuente3);
                }
                break;
        case 3:
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadradoAleatorio,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                }
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->tuberia, miJuego->colorTuberiaBorde, miJuego->colorTuberiaRelleno);
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                break;
        case 4:
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                }
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->tuberia, miJuego->colorTuberiaBorde, miJuego->colorTuberiaRelleno);
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                break;
        case 5:
                MiSDL_PintarTexto(miJuego->miSDL,150,"BONUS ¡COME TODO LO QUE PUEDAS!",miJuego->colorTexto1,miJuego->colorRectanguloBorde1,miJuego->colorRectanguloRelleno1,miJuego->fuente1);
                break;
        case 6:
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadradoAleatorio,miJuego->colorRectanguloBordeEspecial, miJuego->colorRectanguloRellenoEspecial);
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorRectanguloBordeEspecial, miJuego->colorRectanguloRellenoEspecial);
                }
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                break;
        case 7:
                MiSDL_PintarTexto(miJuego->miSDL,150,"OUPS, ¡CHOCASTE !",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                break;
        case 8:
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                }
                MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->tuberia, miJuego->colorTuberiaBorde, miJuego->colorTuberiaRelleno);
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                break;
        case 9:
                for(snakeCeldaAp aux=miJuego->serpienteUltima; aux!=NULL; aux =aux->cuadradoSIG )
                {
                    miJuego->cuadrado.x = aux->coordenadaX;
                    miJuego->cuadrado.y = aux->coordenadaY;
                    if(miJuego->faseAnterior==6)
                            MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorRectanguloBordeEspecial, miJuego->colorRectanguloRellenoEspecial);
                    else
                    MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL,miJuego->cuadrado,miJuego->colorCuadradoBorde,miJuego->colorCuadradoRelleno);
                }
                sprintf(puntua, "%d", miJuego->puntuacion);
                MiSDL_PintarTexto(miJuego->miSDL, 0, puntua, miJuego->colorTextoMarcador, miJuego->colorRectanguloBordeMarcador,miJuego->colorRectanguloRellenoMarcador, miJuego->fuente1);
                MiSDL_PintarTexto(miJuego->miSDL,200,"JUEGO EN PAUSA",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                    break;
    }
    MiSDL_ActualizarVentana(miJuego->miSDL);
}

void MiJuego_EventoTexto(MiJuegoAp miJuego, char * texto)
    {
        if (strlen(miJuego->nombre)+strlen(texto)<TAMNOMBRE) strcat(miJuego->nombre,texto);
    }

MiJuego  MiJuego_Iniciar(char * titulo,
                         int ancho,
                         int alto,
                         int lado,
                         SDL_Color colorFondo,
                         SDL_Color colorCuadradoBorde,
                         SDL_Color colorCuadradoRelleno,
                         int retardo,
                         int retardoMin,
                         int tramos_x,
                         int tramos_y,
                         SDL_Color colorArbol,
                         char * ficheroFuente1,
                         char * ficheroFuente2,
                         char * ficheroFuente3,
                         int tamFuente1,
                         int tamFuente2,
                         int tamFuente3,
                         SDL_Color colorTexto1,
                         SDL_Color colorRectanguloBorde1,
                         SDL_Color colorRectanguloRelleno1,
                         SDL_Color colorTexto2,
                         SDL_Color colorRectanguloBorde2,
                         SDL_Color colorRectanguloRelleno2,
                         SDL_Color colorTexto3,
                         SDL_Color colorRectanguloBorde3,
                         SDL_Color colorRectanguloRelleno3,
                         char * nombreFichero,
                         int nPasosMax,
                         int tiempoEspera,
                         int puntuacionGanadora,int superRetardoMin,
                         SDL_Color colorTuberiaBorde,
                         SDL_Color colorTuberiaRelleno,
                         SDL_Color colorTextoMarcador,
                         SDL_Color colorRectanguloBordeMarcador,
                         SDL_Color colorRectanguloRellenoMarcador,
                         SDL_Color colorRectanguloBordeEspecial,
                         SDL_Color colorRectanguloRellenoEspecial)
    {
    if (ancho<=0) MiJuego_Error("El ancho de la ventana debe ser mayor que cero.");
    if (alto <=0) MiJuego_Error("El alto de la ventana debe de ser mayor  que cero");
    if ((lado >= ancho) || (lado >=alto)) MiJuego_Error("El lado debe de ser menor que el ancho y alto");

    MiJuegoAp miJuego = malloc(sizeof(MiJuegoRep));
    miJuego->miSDL = MiSDL_Iniciar(titulo, ancho,alto);
    miJuego->ancho = ancho;
    miJuego->alto = alto;
    miJuego->cuadrado.w=lado;
    miJuego->cuadrado.h=lado;
    miJuego->cuadradoAleatorio.h = lado;
    miJuego->cuadradoAleatorio.w = lado;
    miJuego->colorCuadradoBorde=colorCuadradoBorde;
    miJuego->colorCuadradoRelleno=colorCuadradoRelleno;
    miJuego->colorRectanguloBordeEspecial=  colorRectanguloBordeEspecial;
    miJuego->colorRectanguloRellenoEspecial= colorRectanguloRellenoEspecial;
    miJuego->retardo=retardo;
    miJuego->retardoMin=retardoMin;
    miJuego->fuente1=MiSDL_CrearFuente(ficheroFuente1,tamFuente1);
    miJuego->fuente2=MiSDL_CrearFuente(ficheroFuente2,tamFuente2);
    miJuego->fuente3=MiSDL_CrearFuente(ficheroFuente3,tamFuente3);
    miJuego->colorTexto1=colorTexto1;
    miJuego->colorRectanguloBorde1=colorRectanguloBorde1;
    miJuego->colorRectanguloRelleno1=colorRectanguloRelleno1;
    miJuego->colorTexto2=colorTexto2;
    miJuego->colorRectanguloBorde2=colorRectanguloBorde2;
    miJuego->colorRectanguloRelleno2=colorRectanguloRelleno2;
    miJuego->colorTexto3=colorTexto3;
    miJuego->colorRectanguloBorde3=colorRectanguloBorde3;
    miJuego->colorRectanguloRelleno3=colorRectanguloRelleno3;
    strcpy(miJuego->nombreFichero,nombreFichero);
    miJuego->nPasosMax = nPasosMax;
    miJuego->puntuacion = 0;
    miJuego->tiempoEspera = tiempoEspera;
    miJuego->puntuacionGanadora = puntuacionGanadora;
    miJuego->superRetardoMin = superRetardoMin;
    miJuego->colorTuberiaBorde = colorTuberiaBorde;
    miJuego->colorTuberiaRelleno = colorTuberiaRelleno;
    miJuego->tuberia.h = lado;
    miJuego->tuberia.w = lado;
    miJuego->tuberia.x = (ancho - 2*lado); //En esta posición ponemos la tubería
    miJuego->tuberia.y = 0; //Coordenada inicial
    miJuego->fase = 0;
    miJuego->colorTextoMarcador=colorTextoMarcador;
    miJuego->colorRectanguloBordeMarcador = colorRectanguloBordeMarcador;
    miJuego->colorRectanguloRellenoMarcador = colorRectanguloRellenoMarcador;
    miJuego->pausa = 0;
    FILE * ficheroRecord;
    ficheroRecord = fopen(nombreFichero,"r");
    for(int i=0;i<TAMUSUARIOS;i++)
        fscanf(ficheroRecord,"%s %d", miJuego->usuarios[i].nombre,&miJuego->usuarios[i].tiempo);
    fclose(ficheroRecord);

    MiJuego_IniciarFase(miJuego,0);
    MiArbol_PintarSelva(miJuego->miSDL,miJuego->ancho,miJuego->alto,tramos_x,tramos_y,colorArbol,colorFondo);
    MiSDL_CrearTextura(miJuego->miSDL);
    return miJuego;
    }

void MiJuego_Terminar(MiJuego  _miJuego)
    {
    MiJuegoAp miJuego = _miJuego;
    MiSDL_LiberarTextura(miJuego->miSDL);
    MiSDL_LiberarFuente(miJuego->fuente1);
    MiSDL_LiberarFuente(miJuego->fuente2);
    MiSDL_LiberarFuente(miJuego->fuente3);
    MiSDL_Terminar(miJuego->miSDL);
    free(miJuego);
    }

void MiJuego_Bucle(MiJuego  _miJuego)
    {
    MiJuegoAp miJuego = _miJuego;
    int terminar = 0;
    SDL_Event evento;
    // Repite mientras no se cierre la aplicación.
    while (!terminar)
      {
        // Repite mientras queden eventos en la cola.
        while (SDL_PollEvent(&evento))
        {
            switch(evento.type)
            {
                case SDL_KEYDOWN: MiJuego_EventoTeclado(miJuego,evento.key.keysym.sym);
                                break;
                case SDL_TEXTINPUT: MiJuego_EventoTexto(miJuego,evento.edit.text);
                                break;
                case SDL_QUIT: terminar = 1;
             }
        }
        if ((miJuego->fase==1) || (miJuego->fase==3) || (miJuego->fase==4) || (miJuego->fase==6) || (miJuego->fase==8)) MiJuego_Mover(miJuego);
        if (miJuego->fase==5 || miJuego->fase==7) MiJuego_TiempoEspera(miJuego);
        MiJuego_Pintar(miJuego);
        if (miJuego->fase==1 || miJuego->fase==3 || miJuego->fase==4 || miJuego->fase==6 || miJuego->fase ==8) SDL_Delay(miJuego->retardo);
      }
    }

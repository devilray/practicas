#include "MiJuego.h"
#include "miSDL.h"
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

    typedef struct
    {
        MiSDL  miSDL;
        int ancho, alto;
        SDL_Rect cuadrado;
        SDL_Color  colorCuadradoBorde, colorCuadradoRelleno;
        int dx,dy;
        int retardo;
        int fase;
        int tiempo;
        char nombre[TAMNOMBRE];
        Usuario usuarios[TAMUSUARIOS];
        TTF_Font * fuente1, * fuente2, * fuente3;
        SDL_Color colorTexto1,colorRectanguloBorde1,colorRectanguloRelleno1;
        SDL_Color colorTexto2,colorRectanguloBorde2,colorRectanguloRelleno2;
        SDL_Color colorTexto3,colorRectanguloBorde3,colorRectanguloRelleno3;
        char nombreFichero[40];
    } MiJuegoRep;
typedef MiJuegoRep * MiJuegoAp;

void MiJuego_Error(char * error)
    {
        printf("%s",error);
        exit(EXIT_FAILURE);
    }


void MiJuego_IniciarFase(MiJuegoAp miJuego, int fase)
{
    miJuego->fase = fase;
    switch(fase)
    {
                case 0:
                    miJuego->cuadrado.x=0;
                    miJuego->cuadrado.y=0;
                    break;
                 case 1:
                    miJuego->tiempo = time(NULL);
                    break;
                 case 2:
                    strcpy(miJuego->nombre,"");
                    SDL_StartTextInput();
                    break;
    }
}

void MiJuego_TerminarFase(MiJuegoAp miJuego)
{
     switch(miJuego->fase)
     {
        case 0: MiJuego_IniciarFase(miJuego,1);  break;
        case 1: MiJuego_IniciarFase(miJuego,0);
                miJuego->tiempo = time(NULL) - miJuego->tiempo;
                if (miJuego->tiempo > miJuego->usuarios[TAMUSUARIOS-1].tiempo)
                    MiJuego_IniciarFase(miJuego,2);
                else MiJuego_IniciarFase(miJuego,0);
                break;
        case 2: SDL_StopTextInput();
                int i=TAMUSUARIOS-1;
                while ((i>0)&&(miJuego->tiempo > miJuego->usuarios[i-1].tiempo) && (i>=0))
                {
                    strcpy(miJuego->usuarios[i].nombre,miJuego->usuarios[i-1].nombre);
                    miJuego->usuarios[i].tiempo = miJuego->usuarios[i-1].tiempo;
                    i--;
                }
                strcpy(miJuego->usuarios[i].nombre,miJuego->nombre);
                miJuego->usuarios[i].tiempo = miJuego->tiempo;
                FILE * ficheroRecord;
                if ((ficheroRecord = fopen(miJuego->nombreFichero,"w"))==NULL) MiJuego_Error(strerror(errno));
                for(int i=0;i<TAMUSUARIOS;i++) fprintf(ficheroRecord,"%s %d", miJuego->usuarios[i].nombre,miJuego->usuarios[i].tiempo);
                fclose(ficheroRecord);
                MiJuego_IniciarFase(miJuego,0);
     }
}

    void MiJuego_EventoTeclado(MiJuegoAp miJuego, SDL_Keycode code)
{
    switch(miJuego->fase){
        case 0: if ((code==SDLK_LEFT) || (code==SDLK_RIGHT) || (code==SDLK_UP) ||(code==SDLK_DOWN))
                    MiJuego_TerminarFase(miJuego);
        case 1:
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
                }
        case 2:
            if ((code==SDLK_RETURN)&&(strlen(miJuego->nombre)>0)) MiJuego_TerminarFase(miJuego);
            if ((code==SDLK_BACKSPACE)&&(strlen(miJuego->nombre)>0)) miJuego->nombre[strlen(miJuego->nombre)-1] = '\0';
    }
}

int MiJuego_Chocar(MiJuegoAp miJuego)
{
    if((miJuego->cuadrado.x < 0) || (miJuego->cuadrado.y <0) || (miJuego->cuadrado.x >= miJuego->ancho) || (miJuego->cuadrado.y >= miJuego->alto)) return 1;
    return 0;
}

void MiJuego_Mover(MiJuegoAp miJuego)
{
    miJuego->cuadrado.x += miJuego->dx * miJuego->cuadrado.w;
    miJuego->cuadrado.y += miJuego->dy * miJuego->cuadrado.h;

    if(MiJuego_Chocar(miJuego)) MiJuego_TerminarFase(miJuego);
}
void MiJuego_Pintar(MiJuegoAp miJuego)
{
    MiSDL_CopiarTextura(miJuego->miSDL);
    switch (miJuego->fase)
    {
        case 0: MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadrado,miJuego->colorCuadradoBorde, miJuego->colorCuadradoRelleno);
                for (int i=0;i<TAMUSUARIOS;i++)
                {
                    char aux[100];
                    sprintf(aux,"%d : %s - %d seg.",i+1,miJuego->usuarios[i].nombre,miJuego->usuarios[i].tiempo);
                    MiSDL_PintarTexto(miJuego->miSDL,100+i*50,aux,miJuego->colorTexto1,miJuego->colorRectanguloBorde1,miJuego->colorRectanguloRelleno1,miJuego->fuente1);
                }
                MiSDL_PintarTexto(miJuego->miSDL,200+(TAMUSUARIOS*50),"Pulse FLECHAS para JUGAR",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                break;
        case 1: MiSDL_PintarRectanguloBordeRelleno(miJuego->miSDL, miJuego->cuadrado,miJuego->colorCuadradoBorde, miJuego->colorCuadradoRelleno);
                break;
        case 2: {
                    char aux[100];
                    sprintf(aux, "Tiempo = %d seg.",miJuego->tiempo);
                    MiSDL_PintarTexto(miJuego->miSDL,250,aux,miJuego->colorTexto1,miJuego->colorRectanguloBorde1,miJuego->colorRectanguloRelleno1,miJuego->fuente1);
                    MiSDL_PintarTexto(miJuego->miSDL,300,"Introduce nombre:",miJuego->colorTexto2,miJuego->colorRectanguloBorde2,miJuego->colorRectanguloRelleno2,miJuego->fuente2);
                    MiSDL_PintarTextoEntrada(miJuego->miSDL,350,400,60,miJuego->nombre,miJuego->colorTexto3,miJuego->colorRectanguloBorde3,miJuego->colorRectanguloRelleno3,miJuego->fuente3);
                }
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
                          char * nombreFichero)
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
    miJuego->colorCuadradoBorde=colorCuadradoBorde;
    miJuego->colorCuadradoRelleno=colorCuadradoRelleno;
    miJuego->retardo=retardo;

    // Crear fuentes.
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

    FILE * ficheroRecord;
    if ((ficheroRecord = fopen(nombreFichero,"r"))==NULL) MiJuego_Error(strerror(errno));
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
                // Resto de eventos.
                // case tipo_evento_1 : administrador_evento_1;
                // case tipo_evento_2 : administrador_evento_2;
                // ...
                // case tipo_evento_n : administrador_evento_n;
            }
        }
        if (miJuego->fase==1) MiJuego_Mover(miJuego);
        MiJuego_Pintar(miJuego);
        if (miJuego->fase==1) SDL_Delay(miJuego->retardo);
    }
}

//incluir




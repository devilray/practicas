#include "MiSDL.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
    {
        SDL_Window * window;
        SDL_Surface * surface;
        SDL_Renderer * renderer;
        SDL_Texture * texture;
    } MiSDLRep;
typedef MiSDLRep* MiSDLAp;

//Errores de TTF y MiSDL


void MiSDL_Error()
    {
        printf("%s",SDL_GetError());
        exit(EXIT_FAILURE);
    }


void MiSDL_TTF_Error()
    {
        printf("%s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

MiSDL  MiSDL_Iniciar(char titulo[], int ancho,int alto)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) MiSDL_Error();
    if (TTF_Init()<0) MiSDL_TTF_Error();
    MiSDLAp miSDL=malloc(sizeof(MiSDLRep));
    miSDL->window = SDL_CreateWindow(titulo,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ancho,alto,0);
    if(miSDL -> window==NULL) MiSDL_Error();
    miSDL->surface= SDL_GetWindowSurface(miSDL->window);
    if(miSDL->surface==NULL) MiSDL_Error();
    miSDL->renderer=SDL_CreateSoftwareRenderer(miSDL->surface);
    if (miSDL->renderer==NULL) MiSDL_Error();
    return miSDL;
}
void MiSDL_Terminar(MiSDL  _miSDL)
    {
        MiSDLAp  miSDL = _miSDL;
        SDL_DestroyRenderer(miSDL->renderer);
        SDL_DestroyWindow(miSDL->window);
        TTF_Quit();
        SDL_Quit();
        free(miSDL);
    }
void MiSDL_LimpiarRender(MiSDL  _miSDL, SDL_Color color)
    {
        MiSDLAp  miSDL = _miSDL ;
        if(SDL_SetRenderDrawColor(miSDL->renderer, color.r,color.g,color.b,color.a) < 0) MiSDL_Error();
        if(SDL_RenderClear(miSDL->renderer) < 0) MiSDL_Error();
    }

void MiSDL_ActualizarVentana(MiSDL  _miSDL)
    {
        MiSDLAp  miSDL = _miSDL;
        if(SDL_UpdateWindowSurface(miSDL->window) < 0) MiSDL_Error();
    }
void MiSDL_PintarRectanguloBordeRelleno(MiSDL  _miSDL, SDL_Rect rectangulo, SDL_Color colorBorde, SDL_Color colorRelleno)
    {
        MiSDLAp  miSDL = _miSDL;
        if(SDL_SetRenderDrawColor(miSDL->renderer,colorRelleno.r,colorRelleno.g,colorRelleno.b,colorRelleno.a) < 0) MiSDL_Error();
        if(SDL_RenderFillRect(miSDL->renderer, &rectangulo) < 0) MiSDL_Error();
        if(SDL_SetRenderDrawColor(miSDL->renderer, colorBorde.r,colorBorde.g,colorBorde.b,colorBorde.a) < 0) MiSDL_Error();
        if(SDL_RenderDrawRect(miSDL->renderer,&rectangulo) < 0) MiSDL_Error();
    }

void MiSDL_PintarLinea(MiSDL  _miSDL, int x1,int y1, int x2,int y2, SDL_Color color)
    {
        MiSDLAp  miSDL = _miSDL;
        if(SDL_SetRenderDrawColor(miSDL->renderer, color.r,color.g,color.b,color.a) < 0) MiSDL_Error();
        if(SDL_RenderDrawLine(miSDL->renderer, x1, y1, x2, y2) <0) MiSDL_Error();
    }

void MiSDL_CrearTextura(MiSDL  _miSDL)
    {
        MiSDLAp  miSDL = _miSDL;
        if ((miSDL->texture=SDL_CreateTextureFromSurface(miSDL->renderer, miSDL->surface))==NULL) MiSDL_Error();
    }

void MiSDL_LiberarTextura(MiSDL  _miSDL)
    {
        MiSDLAp  miSDL = _miSDL;
        SDL_DestroyTexture(miSDL->texture);
    }

void MiSDL_CopiarTextura(MiSDL  _miSDL)
    {
        MiSDLAp  miSDL = _miSDL;
        if( SDL_RenderCopy(miSDL->renderer, miSDL->texture, NULL, NULL)<0) MiSDL_Error();
    }

 TTF_Font * MiSDL_CrearFuente(const char * ficheroFuente, int tamFuente)
    {
        TTF_Font * fuente = TTF_OpenFont(ficheroFuente,tamFuente);
        if (fuente == NULL) MiSDL_TTF_Error();
        return fuente;
    }

 void MiSDL_LiberarFuente(TTF_Font * fuente)
    {
        TTF_CloseFont(fuente);
    }

 void MiSDL_PintarTexto(MiSDL  _miSDL,int y,const char * texto,SDL_Color colorTexto,SDL_Color colorBorde,SDL_Color colorRelleno,TTF_Font * fuente)
{
    MiSDLAp miSDL = _miSDL;
    char textoImprimir[100];
    sprintf(textoImprimir," %s ",texto);
    SDL_Surface * surface = TTF_RenderText_Blended(fuente,textoImprimir,colorTexto);
    if (surface == NULL) MiSDL_TTF_Error();
    SDL_Rect rectangulo;
    rectangulo.w = surface->w;
    rectangulo.h = surface->h;
    rectangulo.x = miSDL->surface->w/2-rectangulo.w/2;
    rectangulo.y = y;
    MiSDL_PintarRectanguloBordeRelleno(miSDL,rectangulo,colorBorde,colorRelleno);
    if (SDL_BlitSurface(surface,NULL,miSDL->surface,&rectangulo) < 0) MiSDL_Error();
    SDL_FreeSurface(surface);
}

void MiSDL_PintarTextoEntrada(MiSDL  _miSDL,int y,int w,int h,const char * texto,SDL_Color colorTexto,SDL_Color colorBorde,SDL_Color colorRelleno,TTF_Font * fuente)
{
    MiSDLAp miSDL = _miSDL;
    char textoImprimir[100];
    sprintf(textoImprimir," %s ",texto);
    SDL_Rect rectangulo;
    rectangulo.w = w;
    rectangulo.h = h;
    rectangulo.x = miSDL->surface->w/2-rectangulo.w/2;
    rectangulo.y = y;
    SDL_Surface * surface = TTF_RenderText_Blended(fuente,textoImprimir,colorTexto);
    if (surface == NULL) MiSDL_TTF_Error();
    MiSDL_PintarRectanguloBordeRelleno(miSDL,rectangulo,colorBorde,colorRelleno);
    if (SDL_BlitSurface(surface,NULL,miSDL->surface,&rectangulo) < 0) MiSDL_Error();
    SDL_FreeSurface(surface);
}

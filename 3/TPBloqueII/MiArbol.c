#include "MiArbol.h"
#include "MiSDL.h"
#include <math.h>
#define PI 3.1416
#define HMIN 15
#include <stdio.h>
#include <stdlib.h>




int MiArbol_Aleatorio(int a, int b)
    {

        return a+rand()%(b-a+1);
    }

void MiArbol_PintarArbol(MiSDL  miSDL, int h, int x, int y, double angulo, SDL_Color color)
{
    if(h>=HMIN)
    {
         int xFinal=x +h*cos(angulo);
         int yFinal=y-h*sin(angulo);
         for (int i=0; i<h/HMIN;i++)
         {
           MiSDL_PintarLinea(miSDL,x+i,y,xFinal+i,yFinal,color);
         }
         SDL_Color colorAclarado=color;
         if(colorAclarado.r>0)
         {
          colorAclarado.r+=20;
          if(colorAclarado.r>=255) colorAclarado.r=255;
         }
          if(colorAclarado.g>0)
         {
          colorAclarado.g+=20;
          if(colorAclarado.g>=255) colorAclarado.g=255;
         }
          if(colorAclarado.b>0)
         {
          colorAclarado.b+=20;
          if(colorAclarado.b>=255) colorAclarado.b=255;
         }
         int n=MiArbol_Aleatorio(1,7);
         for (int j=1; j<=n;j++)
         {
             int hNueva = MiArbol_Aleatorio(h,1.1*h)*2.0/3.0;
             int xNueva = xFinal;
             int yNueva = yFinal;
             double anguloNuevo = angulo-PI/2-PI/(2*n)+j*PI/n;
             SDL_Color colorNuevo = colorAclarado;
             MiArbol_PintarArbol(miSDL, hNueva,xNueva,yNueva,anguloNuevo,colorNuevo);
         }
    }
}

void MiArbol_PintarSelva(MiSDL  miSDL, int ancho,int alto, int tramos_x, int tramos_y, SDL_Color colorArbol, SDL_Color colorFondo)
{
    MiSDL_LimpiarRender(miSDL,colorFondo);
    int anchoCuadricula = ancho/tramos_x;
    int altoCuadricula = alto/tramos_y;
    for(int i=0;i<ancho; i+=anchoCuadricula)
    {
        for(int j=0;j<alto;j+=altoCuadricula)
        {
                MiArbol_PintarArbol(miSDL,
                                    MiArbol_Aleatorio(anchoCuadricula/2,anchoCuadricula),
                                    MiArbol_Aleatorio(i,i+anchoCuadricula),
                                    MiArbol_Aleatorio(j,j+altoCuadricula),
                                    PI/2,colorArbol);
        }
    }
}




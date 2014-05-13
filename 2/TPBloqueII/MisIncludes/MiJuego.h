//
// Fichero MiJuego.h
//

#ifndef __MIJUEGO_H
#define __MIJUEGO_H

#include "MiSDL.h"

typedef void * MiJuego;

MiJuego MiJuego_Iniciar(char * titulo,
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
                        char * nombreFichero);

void MiJuego_Terminar(MiJuego miJuego);
void MiJuego_Bucle(MiJuego miJuego);

#endif // __MIJUEGO_H


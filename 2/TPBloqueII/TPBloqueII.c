#include <stdlib.h>
#include <stdio.h>
#include "MiSDL.h"
#include "MiJuego.h"
#include "MiArbol.h"
#include <time.h>


int main(int argc, char ** argv)
{
    srand(time(NULL));

    SDL_Color color = {232,224,79,SDL_ALPHA_OPAQUE};
    SDL_Color colorBorde = {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorRelleno= {255,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorArbol= {10,131,37,SDL_ALPHA_OPAQUE};
    SDL_Color colorTexto1= {30,30,30,SDL_ALPHA_OPAQUE};
    SDL_Color colorTexto2= {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorTexto3= {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloBorde1= {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloBorde2= {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloBorde3= {0,0,0,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloRelleno1= {145,242,220,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloRelleno2= {223,57,153,SDL_ALPHA_OPAQUE};
    SDL_Color colorRectanguloRelleno3= {32,249,48,SDL_ALPHA_OPAQUE};

    MiJuego miJuego= MiJuego_Iniciar("Mi Juego",500,500,20,color,colorBorde,colorRelleno,200,100,5,5,colorArbol,"arial.ttf","arial.ttf","arial.ttf",25,35,35,colorTexto1,colorRectanguloBorde1,colorRectanguloRelleno1,colorTexto2,colorRectanguloBorde2,colorRectanguloRelleno2,colorTexto3,colorRectanguloBorde3,colorRectanguloRelleno3,"records.txt",3);

    MiJuego_Bucle(miJuego);

    MiJuego_Terminar(miJuego);
    return 0;
}

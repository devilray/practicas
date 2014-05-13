//
// Fichero MiSDL.h
//

#ifndef __MISDL_H
#define __MISDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef void * MiSDL;

MiSDL MiSDL_Iniciar(char titulo[], int ancho, int alto);
void MiSDL_Terminar(MiSDL miSDL);
void MiSDL_LimpiarRender(MiSDL miSDL, SDL_Color color);
void MiSDL_ActualizarVentana(MiSDL miSDL);
void MiSDL_PintarRectanguloBordeRelleno(MiSDL miSDL,
                                        SDL_Rect rectangulo,
                                        SDL_Color colorBorde,
                                        SDL_Color colorRelleno);
void MiSDL_PintarLinea(MiSDL miSDL,
                            int x1,
                            int y1,
                            int x2,
                            int y2,
                    SDL_Color color);
void MiSDL_CrearTextura(MiSDL miSDL);
void MiSDL_LiberarTextura(MiSDL miSDL);
void MiSDL_CopiarTextura(MiSDL miSDL);
TTF_Font * MiSDL_CrearFuente(const char * ficheroFuente, int tamFuente);
void MiSDL_LiberarFuente(TTF_Font * fuente);
void MiSDL_PintarTexto(MiSDL miSDL,
                        int y,
                        const char * texto,
                        SDL_Color colorTexto,
                        SDL_Color colorBorde,
                        SDL_Color colorRelleno,
                        TTF_Font * fuente);
void MiSDL_PintarTextoEntrada(MiSDL miSDL,
                                int y,
                                int w,
                                int h,
                                const char * texto,
                                SDL_Color colorTexto,
                                SDL_Color colorBorde,
                                SDL_Color colorRelleno,
                                TTF_Font * fuente);
void   MiSDL_PintarImagen(MiSDL miSDL, int coordenadaX, int coordenadaY, int anchura,int altura, char * rutaImagen);

#endif // __MISDL_H


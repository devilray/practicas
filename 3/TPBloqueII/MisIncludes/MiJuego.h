/**
\file MiJuego.h
\brief M�dulo principal.
\author Jos� Mar�a Belando L�pez y Francisco Mu�oz Mart�nez
\version 1.0
*/

#ifndef __MIJUEGO_H
#define __MIJUEGO_H

#include "MiSDL.h"

/**
\brief TDA MiJuego
*/
typedef void * MiJuego;

/**
\brief Construye una instancia MiJuego con los valores especificados.
 Provoca un mensaje de error y terminaci�n en los siguientes casos:
 - Si ancho es menor o igual que 0.
 - Si alto es menor o igual que 0.
 - Si lado es mayor o igual que ancho.
 - Si lado es mayor o igual que alto.
\param titulo T�tulo del juego.
\param ancho Ancho de la ventana de juego.
\param alto Alto de la ventana de juego.
\param lado Lado del cuadrado que forma la serpiente.
\param colorFondo Color del fondo de la ventana de juego.
\param colorCuadradoBorde Color del borde del cuadrado que forma la serpiente.
\param colorCuadradoRelleno Color de relleno del cuadrado que forma la serpiente.
\param retardo Retardo del movimiento de la serpiente.
\param retardoMin Retardo m�nimo del movimiento de la serpiente
\param tramos_x N�mero de cuadr�culas horizontales.
\param tramos_y N�mero de cuadr�culas verticales.
\param colorArbol Color inicial de los �rboles y ramas.
\param ficheroFuente1 Nombre del fichero de la fuente 1.
\param ficheroFuente2 Nombre del fichero de la fuente 2.
\param ficheroFuente3 Nombre del fichero de la fuente 3.
\param tamFuente1 Tama�o de la fuente 1.
\param tamFuente2 Tama�o de la fuente 2.
\param tamFuente3 Tama�o de la fuente 3.
\param colorTexto1 Color del texto 1.
\param colorRectanguloBorde1 Color del borde del rect�ngulo del texto 1.
\param colorRectanguloRelleno1 Color del relleno del rect�ngulo del texto 1.
\param colorTexto2 Color del texto 2.
\param colorRectanguloBorde2 Color del borde del rect�ngulo del texto 2.
\param colorRectanguloRelleno2 Color del relleno del rect�ngulo del texto 2.
\param colorTexto3 Color del texto 3.
\param colorRectanguloBorde3 Color del borde del rect�ngulo del texto 3.
\param colorRectanguloRelleno3 Color del relleno del rect�ngulo del texto 3.
\param nombreFichero Nombre del fichero de los records.
\param nPasosMax El n�mero de pasos para aumentar el retardo en el bonus
\param tiempoEspera El tiempo de transici�n en segundos al cambio al y del bonus
\param puntuacionGanadora ontiene los puntos a partir de los cual se iniciar� el bonus
\param superRetardoMin Retardo m�nimo en el bonus
\param colorTuberiaBorde Color del borde del rect�ngulo de entrada al bonus
\param colorTuberiaRelleno Color del relleno del rect�ngulo de entrada al bonus
\param colorTextoMarcador Color del texto del marcador
\param colorRectanguloBordeMarcador Color del borde del rect�ngulo del marcador
\param colorRectanguloRellenoMarcador Color del relleno del rect�ngulo del marcador
\param colorRectanguloBordeEspecial Color borde del rect�ngulo de la serpiente en bonus
\param colorRectanguloRellenoEspecial Color borde del relleno de la serpiente en bonus
\return La instancia MiJuego creada.
*/
MiJuego MiJuego_Iniciar(char * titulo,
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
                        int nPasosMax, int tiempoEspera,
                        int puntuacionGanadora, int superRetardoMin,
                        SDL_Color colorTuberiaBorde,
                        SDL_Color colorTuberiaRelleno,
                        SDL_Color colorTextoMarcador,
                        SDL_Color colorRectanguloBordeMarcador,
                        SDL_Color colorRectanguloRellenoMarcador,
                        SDL_Color colorRectanguloBordeEspecial,
                        SDL_Color colorRectanguloRellenoEspecial);

/**
\brief Libera el entorno gr�fico miSDL asociado a miJuego, las texturas
 y las fuentes.
\param miJuego Entorno.
*/
void MiJuego_Terminar(MiJuego miJuego);

/**
\brief Bucle para el control de eventos sobre miJuego.
\param miJuego Entorno.
*/
void MiJuego_Bucle(MiJuego miJuego);

#endif // __MIJUEGO_H

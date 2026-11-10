#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include "simulador.h" //incluye el archivo simulador.h para poder utilizar las estructuras para poder validar


//validacion de entradas

int leer_entero(int minimo, int maximo); 
double leer_real(double minimo, double maximo);

//validacion de parametros fisicos

int validar_parametros(Parametros parametros); 

//mensaje error 

void mostrar_error(void);



#endif 
#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "simulador.h" //incluye simulador.h para acceder a las estructuras Simulacion, Parametros y Resultados

//FUNCIONES DE GESTION DEL HISTORIAL

void consultar_historial(Simulacion historial[], int cantidadSimulaciones); //permite consultar simulaciones anteriores
void comparar_simulaciones(Simulacion historial[], int cantidadSimulaciones); //permite comparar diferentes simulaciones

#endif

#include <stdio.h>
#include "simulador.h"

int main(void)
{ 
    Parametros parametrosActuales;
    Simulacion historial[capacidad_historial];

    int cantidadSimulaciones = 0;
    int opcion;

    //carga de datos del problema original
    parametrosActuales = cargar_parametros_originales();

    //menu principal 
    do 
    {
        printf("\n===============================================\n");
        printf("       SIMULADOR DE MISIONES ARTEMIS\n");
        printf("========================================\n");
        printf("1. Nueva simulacion\n");
        printf("2. Modificar parametros\n");
        printf("3. Consultar historial\n");
        printf("4. Comparar simulaciones\n");
        printf("5. Salir\n");
        printf("========================================\n");
        opcion = leer_entero(1, 5);
        if (opcion == EOF)
        {
            printf("Error, ingrese numero valido\n");
            break;
        }
        switch (opcion)
        {
        case 1:
            cantidadSimulaciones = ejecutar_simulacion(parametrosActuales, historial, cantidadSimulaciones);
            break;
        
        case 2: 
            parametrosActuales = modificar_parametros(parametrosActuales);
            break;
        case 3:
            consultar_historial(historial, cantidadSimulaciones);
            break;
        case 4: 
            comparar_simulaciones(historial, cantidadSimulaciones);
            break;
        case 5:
            printf("\nFinalizando simulador...\n");
            break;

        }
    } while (opcion != 5);


    return 0;
}
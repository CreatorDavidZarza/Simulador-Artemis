#include <stdio.h>
#include <stdlib.h>
#include "historial.h"
#include "validaciones.h"

/*
    historial.c - Modulo de gestion del historial de simulaciones

    Contiene las funciones de interaccion con el usuario para:
    - Consultar simulaciones guardadas (parametros y resultados)
    - Comparar dos simulaciones entre si (diferencias)
*/


/* ============================================================
   CONSULTAR HISTORIAL
   ============================================================ */

void consultar_historial(Simulacion historial[], int cantidadSimulaciones)
{
    int numeroSimulacion;
    int opcion;
    int indice; // posicion real en el arreglo (desde 0)
    int i;      // variable para el ciclo for

    if (cantidadSimulaciones == 0)
    {
        system("cls");
        printf("\n===============================================\n");
        printf("            HISTORIAL DE SIMULACIONES          \n");
        printf("===============================================\n");
        printf("\nTodavia no hay simulaciones registradas.\n");
        printf("Realice una simulacion primero (Opcion 1).\n");
        esperar_tecla();
        return;
    }

    system("cls");
    printf("\n===============================================\n");
    printf("            HISTORIAL DE SIMULACIONES          \n");
    printf("===============================================\n");
    printf("Simulaciones guardadas: %d de %d\n\n", cantidadSimulaciones, capacidad_historial);

    for (i = 0; i < cantidadSimulaciones; i++)
    {
        printf("  %d. Simulacion %d - Altura max: %.2f m (%.2f km)\n",
               i + 1, i + 1,
               historial[i].resultados.alturaMaxima,
               historial[i].resultados.alturaMaxima / 1000.0);
    }

    printf("\nSeleccione una simulacion (1 a %d): ", cantidadSimulaciones);
    numeroSimulacion = leer_entero(1, cantidadSimulaciones);

    if (numeroSimulacion == EOF)
    {
        return;
    }

    indice = numeroSimulacion - 1;

    do
    {
        system("cls");
        printf("\n===============================================\n");
        printf("               SIMULACION %d                  \n", numeroSimulacion);
        printf("===============================================\n");
        printf("1. Consultar parametros utilizados\n");
        printf("2. Consultar resultados fisicos\n");
        printf("3. Consultar resumen completo\n");
        printf("4. Volver al menu principal\n");
        printf("===============================================\n");
        printf("Seleccione una opcion: ");

        opcion = leer_entero(1, 4);

        if (opcion == EOF)
        {
            return;
        }

        switch (opcion)
        {
            case 1:
                system("cls");
                printf("\n--- PARAMETROS UTILIZADOS (SIMULACION %d) ---\n\n", numeroSimulacion);
                mostrar_parametros(historial[indice].parametros);
                esperar_tecla();
                break;

            case 2:
                system("cls");
                printf("\n--- RESULTADOS FISICOS (SIMULACION %d) ---\n", numeroSimulacion);
                mostrar_resultados(historial[indice].resultados);
                esperar_tecla();
                break;

            case 3:
                system("cls");
                printf("\n--- RESUMEN COMPLETO DE SIMULACION %d ---\n", numeroSimulacion);
                printf("\n[PARAMETROS DE ENTRADA]\n");
                mostrar_parametros(historial[indice].parametros);
                printf("\n[RESULTADOS CALCULADOS]\n");
                mostrar_resultados(historial[indice].resultados);
                esperar_tecla();
                break;

            case 4:
                // Vuelve directamente, el menu principal se encargara de limpiar la pantalla
                break;
        }

    } while (opcion != 4);
}


/* ============================================================
   COMPARAR SIMULACIONES
   ============================================================ */

void comparar_simulaciones(Simulacion historial[], int cantidadSimulaciones)
{
    int primera;
    int segunda;
    int indicePrimera;
    int indiceSegunda;

    if (cantidadSimulaciones < 2)
    {
        system("cls");
        printf("\n===============================================\n");
        printf("             COMPARAR SIMULACIONES             \n");
        printf("===============================================\n");
        printf("\nSe necesitan al menos 2 simulaciones para comparar.\n");
        printf("Simulaciones registradas actualmente: %d\n", cantidadSimulaciones);
        esperar_tecla();
        return;
    }

    system("cls");
    printf("\n===============================================\n");
    printf("             COMPARAR SIMULACIONES             \n");
    printf("===============================================\n");
    printf("Simulaciones disponibles: %d\n\n", cantidadSimulaciones);

    printf("Seleccione la primera simulacion (1 a %d): ", cantidadSimulaciones);
    primera = leer_entero(1, cantidadSimulaciones);

    if (primera == EOF)
    {
        return;
    }

    do
    {
        printf("Seleccione la segunda simulacion (1 a %d): ", cantidadSimulaciones);
        segunda = leer_entero(1, cantidadSimulaciones);

        if (segunda == EOF)
        {
            return;
        }

        if (primera == segunda)
        {
            printf("\nDebe seleccionar una simulacion diferente a la primera (%d).\n\n", primera);
        }

    } while (primera == segunda);

    indicePrimera = primera - 1;
    indiceSegunda = segunda - 1;

    system("cls");
    printf("\n======================================================================\n");
    printf("                 COMPARATIVA: SIMULACION %d vs SIMULACION %d           \n", primera, segunda);
    printf("======================================================================\n");

    printf("\n--- PARAMETROS DE ENTRADA ---\n");
    printf("%-30s | %-18s | %-18s\n", "PARAMETRO", "SIMULACION 1", "SIMULACION 2");
    printf("----------------------------------------------------------------------\n");
    printf("%-30s | %15.2f m/s2 | %15.2f m/s2\n", "Aceleracion Fase 1", 
           historial[indicePrimera].parametros.aceleracion1, historial[indiceSegunda].parametros.aceleracion1);
    printf("%-30s | %15.2f s    | %15.2f s   \n", "Tiempo separacion SRB", 
           historial[indicePrimera].parametros.tiempoSeparacion, historial[indiceSegunda].parametros.tiempoSeparacion);
    printf("%-30s | %15.2f m/s2 | %15.2f m/s2\n", "Aceleracion Fase 2", 
           historial[indicePrimera].parametros.aceleracion2, historial[indiceSegunda].parametros.aceleracion2);
    printf("%-30s | %15.2f m    | %15.2f m   \n", "Altura de apagado", 
           historial[indicePrimera].parametros.alturaApagado, historial[indiceSegunda].parametros.alturaApagado);
    printf("%-30s | %15.2f s    | %15.2f s   \n", "Intervalo simulacion", 
           historial[indicePrimera].parametros.intervaloSimulacion, historial[indiceSegunda].parametros.intervaloSimulacion);

    printf("\n--- RESULTADOS FISICOS OBTENIDOS ---\n");
    printf("%-30s | %-18s | %-18s\n", "MAGNITUD", "SIMULACION 1", "SIMULACION 2");
    printf("----------------------------------------------------------------------\n");
    printf("%-30s | %15.2f m/s  | %15.2f m/s \n", "Velocidad separacion", 
           historial[indicePrimera].resultados.velocidad1, historial[indiceSegunda].resultados.velocidad1);
    printf("%-30s | %15.2f km   | %15.2f km  \n", "Altura separacion", 
           historial[indicePrimera].resultados.altura1 / 1000.0, historial[indiceSegunda].resultados.altura1 / 1000.0);
    printf("%-30s | %15.2f m/s  | %15.2f m/s \n", "Velocidad al apagado", 
           historial[indicePrimera].resultados.velocidad2, historial[indiceSegunda].resultados.velocidad2);
    printf("%-30s | %15.2f km   | %15.2f km  \n", "Altura maxima (apogeo)", 
           historial[indicePrimera].resultados.alturaMaxima / 1000.0, historial[indiceSegunda].resultados.alturaMaxima / 1000.0);
    printf("%-30s | %15.2f s    | %15.2f s   \n", "Tiempo total", 
           historial[indicePrimera].resultados.tiempoTotal, historial[indiceSegunda].resultados.tiempoTotal);
    printf("%-30s | %15.2f G    | %15.2f G   \n", "Aceleracion Fase 1 (G)", 
           historial[indicePrimera].resultados.multiploGravedad, historial[indiceSegunda].resultados.multiploGravedad);

    printf("\n--- DIFERENCIAS (Sim %d - Sim %d) ---\n", primera, segunda);
    printf("  Diferencia altura maxima:     %+10.2f m  (%+.2f km)\n",
           historial[indicePrimera].resultados.alturaMaxima - historial[indiceSegunda].resultados.alturaMaxima,
           (historial[indicePrimera].resultados.alturaMaxima - historial[indiceSegunda].resultados.alturaMaxima) / 1000.0);
    printf("  Diferencia velocidad apagado: %+10.2f m/s (%+.2f km/h)\n",
           historial[indicePrimera].resultados.velocidad2 - historial[indiceSegunda].resultados.velocidad2,
           (historial[indicePrimera].resultados.velocidad2 - historial[indiceSegunda].resultados.velocidad2) * 3.6);
    printf("  Diferencia tiempo total:      %+10.2f s\n",
           historial[indicePrimera].resultados.tiempoTotal - historial[indiceSegunda].resultados.tiempoTotal);
    printf("  Diferencia aceleracion G:     %+10.2f G\n",
           historial[indicePrimera].resultados.multiploGravedad - historial[indiceSegunda].resultados.multiploGravedad);
    printf("======================================================================\n");

    esperar_tecla();
}

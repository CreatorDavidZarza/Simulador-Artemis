#include <stdio.h>
#include <math.h> 
#include "simulador.h"

// 1. FUNCION DE CONFIGURACION INICIAL
Parametros cargar_parametros_originales(void) {
    Parametros p;
    
    // Asignamos las constantes definidas en el .h a nuestra estructura
    p.aceleracion1 = aceleracion1_original;
    p.tiempoSeparacion = tiempo_separacion_original;
    p.aceleracion2 = aceleracion2_original;
    p.alturaApagado = altura_apagado_original;
    p.intervaloSimulacion = intervalo_original;
    
    return p;
}

// 2. FUNCION PRINCIPAL DE SIMULACION (Primera parte)
int ejecutar_simulacion(Parametros parametros, Simulacion historial[], int cantidadSimulaciones) {
    Resultados r; // Acá guardaremos todos los cálculos
    
    printf("\n--- INICIANDO SIMULACION ---\n");

    // FASE 1: Ascenso y Propulsión con SRB (Pregunta 'a' y 'd')
    
    // Cálculo de la rapidez V1: V = V0 + a * t
    r.velocidad1 = velocidadInicial + (parametros.aceleracion1 * parametros.tiempoSeparacion);
    
    // Cálculo de la altitud y1: Y = Y0 + V0*t + 0.5 * a * t^2
    r.altura1 = alturaInicial + (velocidadInicial * parametros.tiempoSeparacion) + 
                (0.5 * parametros.aceleracion1 * pow(parametros.tiempoSeparacion, 2));

    // Cálculo de las Fuerzas G experimentadas
    r.multiploGravedad = parametros.aceleracion1 / gravedad;

    // primeros resultados parciales
    printf("\n[FASE 1] Desacople de cohetes SRB a los %.2f segundos:\n", parametros.tiempoSeparacion);
    printf("- Altitud alcanzada: %.2f metros\n", r.altura1);
    printf("- Velocidad alcanzada: %.2f m/s\n", r.velocidad1);
    printf("- Aceleracion experimentada: %.2f G\n", r.multiploGravedad);

    // FASE 2: Propulsión de la etapa central hasta la altura de apagado (Pregunta 'b')
    // Ecuación sin tiempo: Vf = sqrt(Vi^2 + 2 * a * Δy)
    double distanciaFase2 = parametros.alturaApagado - r.altura1;
    r.velocidad2 = sqrt(pow(r.velocidad1, 2) + 2 * parametros.aceleracion2 * distanciaFase2);
    
    // Tiempo de la fase 2: t = (Vf - Vi) / a
    double tiempoFase2 = (r.velocidad2 - r.velocidad1) / parametros.aceleracion2;
    double tiempoTotalApagado = parametros.tiempoSeparacion + tiempoFase2;

    printf("\n[FASE 2] Apagado de motores a los %.2f metros:\n", parametros.alturaApagado);
    printf("- Tiempo total transcurrido: %.2f segundos\n", tiempoTotalApagado);
    printf("- Velocidad al apagado: %.2f m/s\n", r.velocidad2);

    // FASE 3: Vuelo libre hasta altitud máxima/apogeo (Pregunta 'c')
    // Aceleración = -gravedad. Velocidad final en el apogeo = 0.
    // Altura máxima: Ymax = Yi + (Vi^2) / (2 * g)
    r.alturaMaxima = parametros.alturaApagado + (pow(r.velocidad2, 2) / (2 * gravedad));
    
    // Tiempo de vuelo libre hasta el apogeo: t = Vi / g
    double tiempoFase3 = r.velocidad2 / gravedad;
    double tiempoTotalApogeo = tiempoTotalApagado + tiempoFase3;

    printf("\n[FASE 3] Vuelo libre hasta el Apogeo:\n");
    printf("- Altitud maxima alcanzada: %.2f metros\n", r.alturaMaxima);
    printf("- Tiempo total hasta el apogeo: %.2f segundos\n", tiempoTotalApogeo);
    printf("----------------------------\n");

   // Guardado en el historial
    historial[cantidadSimulaciones].resultados = r;
    historial[cantidadSimulaciones].parametros = parametros;
    
    cantidadSimulaciones++; // Sumamos 1 al contador de simulaciones
    
    return cantidadSimulaciones; 
}

// FUNCION PARA EXPORTAR A CSV
void exportar_a_csv(Simulacion historial[], int cantidadSimulaciones) {
    // Abrimos un archivo en modo escritura ("w" = write)
    FILE *archivo = fopen("resultados_simulacion.csv", "w");
    
    if (archivo == NULL) {
        printf("\n[ERROR] No se pudo crear el archivo CSV.\n");
        return;
    }
    
    // Escribimos la cabecera del archivo (los nombres de las columnas)
    fprintf(archivo, "Num_Simulacion,Aceleracion1,Tiempo_Separacion,Altitud_Fase1,Fuerza_G,Velocidad_Fase2,Altitud_Maxima\n");
    
    // Iteramos sobre el historial para volcar los datos
    for (int i = 0; i < cantidadSimulaciones; i++) {
        fprintf(archivo, "%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                i + 1,
                historial[i].parametros.aceleracion1,
                historial[i].parametros.tiempoSeparacion,
                historial[i].resultados.altura1,
                historial[i].resultados.multiploGravedad,
                historial[i].resultados.velocidad2,
                historial[i].resultados.alturaMaxima);
    }
    
    fclose(archivo); // Siempre cerramos el archivo para liberar memoria
    printf("\n[EXITO] Datos exportados correctamente a 'resultados_simulacion.csv'\n");
}
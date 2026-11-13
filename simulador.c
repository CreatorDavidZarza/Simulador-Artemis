#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "simulador.h"
#include "validaciones.h"

/*
    simulador.c - Modulo de logica fisica del cohete SLS (Artemis 1)

    Contiene las funciones que implementan:
    - Carga y modificacion de parametros del escenario
    - Control de coherencia fisica antes del ingreso de datos
    - Pausa interactiva y ejecucion con cronometro en tiempo real (\r)
    - Control de flujo por fases interactivo (continuar o salir)
    - Calculos fisicos analiticos y simulacion paso a paso (MRUV)
    - Impresion detallada de resultados con conversion de unidades
*/


/* ============================================================
   FUNCIONES AUXILIARES DE FLUJO Y TIEMPO
   ============================================================ */

// Pausa la ejecucion una cantidad de segundos usando la libreria estandar <time.h>
// Permite que la telemetria avance en tiempo real como un cronometro digital visible
void pausar_segundos(double segundos)
{
    clock_t inicio; // Marca de tiempo inicial del procesador
    inicio = clock();

    // Bucle de espera activa hasta transcurrir el tiempo solicitado
    while (((double)(clock() - inicio) / CLOCKS_PER_SEC) < segundos)
    {
        // Espera para dar el efecto de avance en vivo
    }
}

// Pregunta al usuario si desea avanzar a la siguiente fase o cancelar la simulacion
// Retorna 1 si desea continuar, o 0 si decide volver al menu principal
int preguntar_continuacion(const char *mensajeSiguienteFase)
{
    int opcion;

    printf("\n¿Desea continuar con la simulacion?\n");
    printf("  1. Continuar a %s\n", mensajeSiguienteFase);
    printf("  2. Salir y volver al menu principal\n");
    printf("Seleccione una opcion: ");

    opcion = leer_entero(1, 2);
    if (opcion == 2)
    {
        printf("\nSimulacion finalizada por el usuario. Regresando al menu principal...\n");
        esperar_tecla();
        return 0; // Indica al llamador salir al menu
    }

    return 1; // Indica al llamador continuar con la siguiente fase
}


/* ============================================================
   CONFIGURACION DE PARAMETROS
   ============================================================ */

// Carga los valores originales del escenario 1 del Trabajo Final
// a1 = 20.0 m/s2, t_sep = 120.0 s, a2 = 12.5 m/s2, h_apagado = 160000.0 m, dt = 1.0 s
Parametros cargar_parametros_originales(void)
{
    Parametros p;

    p.aceleracion1 = aceleracion1_original;          // 20.0 m/s2
    p.tiempoSeparacion = tiempo_separacion_original; // 120.0 s
    p.aceleracion2 = aceleracion2_original;          // 12.5 m/s2
    p.alturaApagado = altura_apagado_original;       // 160000.0 m
    p.intervaloSimulacion = intervalo_original;      // 1.0 s

    return p;
}

// Permite modificar los parametros de la mision de forma guiada o individual
// Muestra las reglas de coherencia fisica antes de la entrada de datos para orientar al usuario
Parametros modificar_parametros(Parametros parametros)
{
    int opcion;
    Parametros nuevosParametros;
    double altura1_est;

    nuevosParametros = parametros;

    do
    {
        // Calculo de la altura que alcanzara el cohete en la Fase 1 segun los valores actuales
        // Formula MRUV: h = 0.5 * a1 * t^2
        altura1_est = (nuevosParametros.aceleracion1 * nuevosParametros.tiempoSeparacion * nuevosParametros.tiempoSeparacion) / 2.0;

        system("cls");
        printf("==============================================================\n");
        printf("                MODIFICAR PARAMETROS DE MISION               \n");
        printf("==============================================================\n");
        printf("Parametros configurados actualmente:\n\n");
        printf("  1. Aceleracion Fase 1:       %10.2f m/s2\n", nuevosParametros.aceleracion1);
        printf("  2. Tiempo separacion SRB:    %10.2f s    (%.2f min)\n", nuevosParametros.tiempoSeparacion, nuevosParametros.tiempoSeparacion / 60.0);
        printf("  3. Aceleracion Fase 2:       %10.2f m/s2\n", nuevosParametros.aceleracion2);
        printf("  4. Altura de apagado:        %10.2f m    (%.2f km)\n", nuevosParametros.alturaApagado, nuevosParametros.alturaApagado / 1000.0);
        printf("  5. Intervalo simulacion:     %10.2f s\n", nuevosParametros.intervaloSimulacion);
        printf("--------------------------------------------------------------\n");
        printf("  * Altura calculada al separar SRB: %10.2f m (%.2f km)\n", altura1_est, altura1_est / 1000.0);
        
        // Verificacion visual preventiva de consistencia fisica
        if (nuevosParametros.alturaApagado <= altura1_est)
        {
            printf("    [!] AVISO: Incoherencia fisica detectada (Fase 1 supera la altura de apagado)\n");
            printf("               Debe corregirse antes de guardar.\n");
        }
        else
        {
            printf("    [OK] Proporcion fisica consistente (Altura apagado > Altura Fase 1)\n");
        }

        printf("--------------------------------------------------------------\n");
        printf("[i] REGLA DE CONSISTENCIA FISICA (MRUV):\n");
        printf("    En Fase 1 el cohete asciende: h_sep = 0.5 * a1 * (t_sep)^2\n");
        printf("    Por lo tanto, la Altura de apagado (Fase 2) SIEMPRE debe ser\n");
        printf("    MAYOR que la altura alcanzada al separar los propulsores SRB.\n");
        printf("    Ejemplo Artemis 1: Con a1 = 20 m/s2 y t = 120 s -> h_sep = 144.00 km.\n");
        printf("    La altura de apagado debe superar los 144 km (Artemis usa 160.00 km).\n");
        printf("--------------------------------------------------------------\n");
        printf("Acciones disponibles:\n");
        printf("  1. Modificar un parametro individual\n");
        printf("  2. Modificar TODOS los parametros (Asistente guiado)\n");
        printf("  3. Restaurar valores originales de Artemis 1\n");
        printf("  4. Guardar cambios y volver al menu principal\n");
        printf("  5. Cancelar cambios y volver al menu principal\n");
        printf("==============================================================\n");
        printf("Seleccione una opcion: ");

        opcion = leer_entero(1, 5);
        if (opcion == EOF) return parametros;

        switch (opcion)
        {
            case 1:
            {
                int subOpcion;
                printf("\n¿Cual parametro desea cambiar? (1 a 5, o 6 para cancelar): ");
                subOpcion = leer_entero(1, 6);
                if (subOpcion == 1) {
                    printf("Nueva aceleracion Fase 1 (0.01 a 1000.0 m/s2): ");
                    nuevosParametros.aceleracion1 = leer_real(0.01, 1000.0);
                } else if (subOpcion == 2) {
                    printf("Nuevo tiempo de separacion (1.0 a 10000.0 s): ");
                    nuevosParametros.tiempoSeparacion = leer_real(1.0, 10000.0);
                } else if (subOpcion == 3) {
                    printf("Nueva aceleracion Fase 2 (0.01 a 1000.0 m/s2): ");
                    nuevosParametros.aceleracion2 = leer_real(0.01, 1000.0);
                } else if (subOpcion == 4) {
                    printf("Nota: Segun la Fase 1 actual, el cohete llega a %.2f m (%.2f km).\n", altura1_est, altura1_est / 1000.0);
                    printf("Nueva altura de apagado (debe ser mayor a %.2f m): ", altura1_est);
                    nuevosParametros.alturaApagado = leer_real(1.0, 100000000.0);
                } else if (subOpcion == 5) {
                    printf("Nuevo intervalo de simulacion (%.0f a %.0f s): ", intervalo_minimo, intervalo_maximo);
                    nuevosParametros.intervaloSimulacion = leer_real(intervalo_minimo, intervalo_maximo);
                }
                break;
            }

            case 2:
            {
                int confirmar;

                system("cls");
                printf("==============================================================\n");
                printf("        ASISTENTE: INGRESO DE PARAMETROS COMPLETOS            \n");
                printf("==============================================================\n");
                printf("[i] REGLA DE CONSISTENCIA FISICA ANTES DE INGRESAR VALORES:\n");
                printf("    Durante la Fase 1, la altura alcanzada se calcula como:\n");
                printf("        h_separacion = 0.5 * a1 * (t_sep)^2\n");
                printf("    La Altura de apagado (Fase 2) DEBE ser mayor a h_separacion.\n\n");
                printf("    Ejemplo (Mision Artemis 1 original):\n");
                printf("      - Si a1 = 20.00 m/s2 y t_sep = 120.00 s:\n");
                printf("        h_separacion = 0.5 * 20 * (120)^2 = 144,000 m (144.00 km).\n");
                printf("      - La altura de apagado DEBE ser mayor a 144.00 km (ej: 160.00 km).\n");
                printf("--------------------------------------------------------------\n\n");

                printf("1. Ingrese aceleracion Fase 1 (0.01 a 1000.0 m/s2): ");
                nuevosParametros.aceleracion1 = leer_real(0.01, 1000.0);

                printf("2. Ingrese tiempo separacion SRB (1.0 a 10000.0 s): ");
                nuevosParametros.tiempoSeparacion = leer_real(1.0, 10000.0);

                printf("3. Ingrese aceleracion Fase 2 (0.01 a 1000.0 m/s2): ");
                nuevosParametros.aceleracion2 = leer_real(0.01, 1000.0);

                // Se calcula la cota fisica alcanzada con los datos recien ingresados
                altura1_est = 0.5 * nuevosParametros.aceleracion1 * nuevosParametros.tiempoSeparacion * nuevosParametros.tiempoSeparacion;
                printf("\n    --> Proporcion Fase 1 calculada: h_sep = %.2f m (%.2f km)\n", altura1_est, altura1_est / 1000.0);
                printf("4. Ingrese altura de apagado de motores (debe superar %.2f m): ", altura1_est);
                nuevosParametros.alturaApagado = leer_real(altura1_est + 1.0, 100000000.0);

                printf("5. Ingrese intervalo de simulacion (%.0f a %.0f s): ", intervalo_minimo, intervalo_maximo);
                nuevosParametros.intervaloSimulacion = leer_real(intervalo_minimo, intervalo_maximo);

                printf("\n==============================================================\n");
                printf("Todos los parametros fueron ingresados correctamente.\n\n");
                printf("Resumen ingresado:\n");
                printf("  - Fase 1: a1 = %.2f m/s2 | t_sep = %.2f s (h_sep = %.2f km)\n",
                       nuevosParametros.aceleracion1, nuevosParametros.tiempoSeparacion, altura1_est / 1000.0);
                printf("  - Fase 2: a2 = %.2f m/s2 | h_apagado = %.2f km\n",
                       nuevosParametros.aceleracion2, nuevosParametros.alturaApagado / 1000.0);
                printf("  - Intervalo de simulacion: dt = %.2f s\n", nuevosParametros.intervaloSimulacion);
                printf("--------------------------------------------------------------\n");
                printf("¿Desea GUARDAR estos cambios ahora mismo?\n");
                printf("  1. Si, guardar y volver al menu principal\n");
                printf("  2. No, volver al menu de parametros para seguir editando\n");
                printf("Seleccione una opcion: ");
                
                confirmar = leer_entero(1, 2);
                if (confirmar == 1)
                {
                    printf("\nParametros guardados exitosamente.\n");
                    esperar_tecla();
                    return nuevosParametros;
                }
                else
                {
                    printf("\nParametros guardados temporalmente en el borrador.\n");
                    esperar_tecla();
                    break;
                }
            }

            case 3:
                nuevosParametros = cargar_parametros_originales();
                printf("\nValores originales de Artemis 1 restaurados.\n");
                esperar_tecla();
                break;

            case 4:
                // Validar consistencia fisica al guardar definitivamente
                if (validar_parametros(nuevosParametros))
                {
                    printf("\nParametros validados y guardados con exito.\n");
                    esperar_tecla();
                    return nuevosParametros;
                }
                else
                {
                    altura1_est = (nuevosParametros.aceleracion1 * nuevosParametros.tiempoSeparacion * nuevosParametros.tiempoSeparacion) / 2.0;
                    printf("\n--------------------------------------------------------------\n");
                    printf("ERROR: Inconsistencia fisica en los parametros ingresados:\n");
                    printf("  - Con a1 = %.2f m/s2 y t = %.2f s, el cohete alcanza %.2f m (%.2f km) en Fase 1.\n",
                           nuevosParametros.aceleracion1, nuevosParametros.tiempoSeparacion, altura1_est, altura1_est / 1000.0);
                    printf("  - Su altura de apagado configurada es %.2f m (%.2f km).\n",
                           nuevosParametros.alturaApagado, nuevosParametros.alturaApagado / 1000.0);
                    printf("  - REGLA: La altura de apagado debe ser MAYOR a la altura de separacion.\n");
                    printf("--------------------------------------------------------------\n");
                    printf("Ajuste los valores antes de guardar.\n");
                    esperar_tecla();
                }
                break;

            case 5:
                printf("\nModificacion cancelada. Se mantienen los parametros anteriores.\n");
                esperar_tecla();
                return parametros;
        }

    } while (1);
}


/* ============================================================
   SIMULACION PRINCIPAL - CALCULOS FISICOS Y TELEMETRIA
   ============================================================ */

// Ejecuta la simulacion completa de las 3 fases del vuelo del SLS
int ejecutar_simulacion(Parametros parametros, Simulacion historial[], int cantidadSimulaciones)
{
    Resultados resultados;
    double velocidad; 
    double tiempo;         
    double altura;         
    double deltaTiempo;    
    double tiempoRestante; 
    double distanciaFase2; 
    double tiempoFase3;   
    int minutos;
    double segundosRestantes;
    double pausaPaso;

    // Verificar que haya espacio en el historial
    if (cantidadSimulaciones >= capacidad_historial)
    {
        system("cls");
        printf("\nEl historial esta completo (%d simulaciones maximo).\n", capacidad_historial);
        printf("No se pueden guardar mas simulaciones.\n");
        esperar_tecla();
        return cantidadSimulaciones;
    }

    // Verificar que los parametros actuales sean validos
    if (!validar_parametros(parametros))
    {
        system("cls");
        mostrar_error();
        esperar_tecla();
        return cantidadSimulaciones;
    }

    /* ==========================================================
       CALCULOS ANALITICOS PREVIOS (respuestas del TP)
       ========================================================== */

    // a) Velocidad al separar los SRB (fin de fase 1): v = v0 + a * t
    resultados.velocidad1 = velocidadInicial + parametros.aceleracion1 * parametros.tiempoSeparacion;

    // a) Altura al separar los SRB (fin de fase 1): y = y0 + v0*t + 1/2*a*t^2
    resultados.altura1 = alturaInicial + velocidadInicial * parametros.tiempoSeparacion + 0.5 * parametros.aceleracion1 * parametros.tiempoSeparacion * parametros.tiempoSeparacion;

    // Distancia de la Fase 2
    distanciaFase2 = parametros.alturaApagado - resultados.altura1;

    // b) Velocidad al apagar motores: v = sqrt(v0^2 + 2*a*d)
    resultados.velocidad2 = sqrt(resultados.velocidad1 * resultados.velocidad1 + 2.0 * parametros.aceleracion2 * distanciaFase2);

    // Tiempo de la fase 2: t = (v - v0) / a
    resultados.tiempo2 = (resultados.velocidad2 - resultados.velocidad1) / parametros.aceleracion2;

    // c) Altura adicional en vuelo libre: d = v0^2 / (2*g)
    resultados.alturaAdicional = (resultados.velocidad2 * resultados.velocidad2) / (2.0 * gravedad);

    // c) Altura maxima total
    resultados.alturaMaxima = parametros.alturaApagado + resultados.alturaAdicional;

    // Tiempo de la fase 3: t = v / g
    tiempoFase3 = resultados.velocidad2 / gravedad;

    // Tiempo total hasta el apogeo
    resultados.tiempoTotal = parametros.tiempoSeparacion + resultados.tiempo2 + tiempoFase3;

    // d) Aceleracion maxima como multiplo de g
    resultados.multiploGravedad = parametros.aceleracion1 / gravedad;


    /* ==========================================================
       SIMULACION PASO A PASO EN VIVO (CRONOMETRO DIGITAL)
       ========================================================== */

    // Condiciones iniciales del vuelo
    tiempo = 0.0;
    altura = alturaInicial;     
    velocidad = velocidadInicial; 

    // Pausa por cada paso en Fase 1 (35 ms): permite ver el cronometro avanzar con claridad
    pausaPaso = 0.035;

    /* --- FASE 1: ascenso con propulsores SRB --- */
    system("cls");
    printf("==============================================================\n");
    printf("              FASE 1: ASCENSO CON PROPULSORES SRB             \n");
    printf("==============================================================\n");
    printf("Aceleracion constante a1: %.2f m/s2 | Intervalo dt: %.2f s\n",
           parametros.aceleracion1, parametros.intervaloSimulacion);
    printf("Iniciando cronometro de lanzamiento en tiempo real...\n\n");

    while (tiempo < parametros.tiempoSeparacion)
    {
        tiempoRestante = parametros.tiempoSeparacion - tiempo;
        deltaTiempo = parametros.intervaloSimulacion;

        // Ajuste en el ultimo paso para no pasarse del tiempo exacto de separacion
        if (deltaTiempo > tiempoRestante)
        {
            deltaTiempo = tiempoRestante;
        }

        // Ecuaciones de movimiento rectilineo uniformemente variado (MRUV)
        altura = altura + velocidad * deltaTiempo + 0.5 * parametros.aceleracion1 * deltaTiempo * deltaTiempo;
        velocidad = velocidad + parametros.aceleracion1 * deltaTiempo;
        tiempo = tiempo + deltaTiempo;

        // Formato de reloj digital cronometro (minutos:segundos)
        minutos = (int)(tiempo / 60.0);
        segundosRestantes = tiempo - (minutos * 60.0);

        // Telemetria en vivo: \r ubica el cursor al inicio del mismo renglon
        printf("\r  [CRONOMETRO T+ %02d:%05.2f]  Altitud: %8.2f km  |  Velocidad: %8.2f km/h  (%.2f m/s)",
               minutos, segundosRestantes, altura / 1000.0, velocidad * 3.6, velocidad);
        fflush(stdout); // Forza a la consola a refrescar la linea inmediatamente

        // Pausa breve para dar el efecto de avance continuo como un cronometro
        pausar_segundos(pausaPaso);
    }

    // Fijar los valores exactos calculados
    altura = resultados.altura1;
    velocidad = resultados.velocidad1;
    tiempo = parametros.tiempoSeparacion;

    printf("\n\n--------------------------------------------------------------\n");
    printf(">>> EVENTO: SEPARACION DE LOS PROPULSORES SRB <<<\n");
    printf("--------------------------------------------------------------\n");
    printf("  - Tiempo de desacople: %.2f s (%.2f min)\n", tiempo, tiempo / 60.0);
    printf("  - Altitud alcanzada:   %.2f m (%.2f km)\n", altura, altura / 1000.0);
    printf("  - Rapidez vertical:    %.2f m/s (%.2f km/h)\n", velocidad, velocidad * 3.6);
    printf("--------------------------------------------------------------\n");

    // Pausa interactiva: no borra la pantalla y consulta al usuario si desea continuar o salir
    if (!preguntar_continuacion("Fase 2 (Propulsion con etapa central SLS)"))
    {
        return cantidadSimulaciones;
    }


    /* --- FASE 2: propulsion etapa central SLS --- */
    system("cls");
    printf("==============================================================\n");
    printf("         FASE 2: PROPULSION CON ETAPA CENTRAL SLS             \n");
    printf("==============================================================\n");
    printf("Aceleracion constante a2: %.2f m/s2 | Meta apagado: %.2f km\n",
           parametros.aceleracion2, parametros.alturaApagado / 1000.0);
    printf("Continuando ascenso con motores principales RS-25...\n\n");

    while (altura < parametros.alturaApagado)
    {
        distanciaFase2 = parametros.alturaApagado - altura;
        tiempoRestante = (2.0 * distanciaFase2) / (velocidad + sqrt(velocidad * velocidad + 2.0 * parametros.aceleracion2 * distanciaFase2));
        deltaTiempo = parametros.intervaloSimulacion;

        if (deltaTiempo > tiempoRestante)
        {
            deltaTiempo = tiempoRestante;
        }

        altura = altura + velocidad * deltaTiempo + 0.5 * parametros.aceleracion2 * deltaTiempo * deltaTiempo;
        velocidad = velocidad + parametros.aceleracion2 * deltaTiempo;
        tiempo = tiempo + deltaTiempo;

        if (deltaTiempo == tiempoRestante)
        {
            altura = parametros.alturaApagado;
            velocidad = resultados.velocidad2;
        }

        minutos = (int)(tiempo / 60.0);
        segundosRestantes = tiempo - (minutos * 60.0);

        // Telemetria en vivo sobre el mismo renglon usando \r
        printf("\r  [CRONOMETRO T+ %02d:%05.2f]  Altitud: %8.2f km  |  Velocidad: %8.2f km/h  (%.2f m/s)",
               minutos, segundosRestantes, altura / 1000.0, velocidad * 3.6, velocidad);
        fflush(stdout);

        // En Fase 2, como suelen ser pocos pasos, se da un tiempo mayor por paso (100 ms) para poder leerlo
        pausar_segundos(0.10);
    }

    printf("\n\n--------------------------------------------------------------\n");
    printf(">>> EVENTO: APAGADO DE MOTORES PRINCIPALES (MECO) <<<\n");
    printf("--------------------------------------------------------------\n");
    printf("  - Tiempo transcurrido: %.2f s (%.2f min)\n", tiempo, tiempo / 60.0);
    printf("  - Duracion Fase 2:     %.2f s\n", resultados.tiempo2);
    printf("  - Altitud de corte:    %.2f m (%.2f km)\n", altura, altura / 1000.0);
    printf("  - Rapidez maxima v2:   %.2f m/s (%.2f km/h)\n", velocidad, velocidad * 3.6);
    printf("--------------------------------------------------------------\n");

    // Pausa interactiva para Fase 2
    if (!preguntar_continuacion("Fase 3 (Vuelo libre hasta el apogeo)"))
    {
        return cantidadSimulaciones;
    }


    /* --- FASE 3: vuelo libre hasta el apogeo --- */
    system("cls");
    printf("==============================================================\n");
    printf("              FASE 3: VUELO LIBRE HASTA EL APOGEO             \n");
    printf("==============================================================\n");
    printf("Propulsion: APAGADA (inercia). Desaceleracion por gravedad: 9.80 m/s2\n");
    printf("Ascenso libre hasta que la velocidad vertical sea 0.0 m/s...\n\n");

    while (velocidad > 0.0)
    {
        tiempoRestante = velocidad / gravedad;
        deltaTiempo = parametros.intervaloSimulacion;

        if (deltaTiempo > tiempoRestante)
        {
            deltaTiempo = tiempoRestante;
        }

        // Frenado por gravedad segun MRUV
        altura = altura + velocidad * deltaTiempo - 0.5 * gravedad * deltaTiempo * deltaTiempo;
        velocidad = velocidad - gravedad * deltaTiempo;
        tiempo = tiempo + deltaTiempo;

        if (deltaTiempo == tiempoRestante)
        {
            velocidad = 0.0;
            altura = resultados.alturaMaxima;
        }

        minutos = (int)(tiempo / 60.0);
        segundosRestantes = tiempo - (minutos * 60.0);

        // Telemetria en vivo sobre el mismo renglon usando \r
        printf("\r  [CRONOMETRO T+ %02d:%05.2f]  Altitud: %8.2f km  |  Velocidad: %8.2f km/h  (%.2f m/s)",
               minutos, segundosRestantes, altura / 1000.0, velocidad * 3.6, velocidad);
        fflush(stdout);

        // En Fase 3 (aprox 250 pasos) con 25 ms toma unos 6 segundos
        pausar_segundos(0.025);
    }

    printf("\n\n--------------------------------------------------------------\n");
    printf(">>> EVENTO: APOGEO ALCANZADO (VELOCIDAD VERTICAL CERO) <<<\n");
    printf("--------------------------------------------------------------\n");
    printf("  - Tiempo total hasta apogeo:    %.2f s (%.2f min)\n", tiempo, tiempo / 60.0);
    printf("  - Altura maxima h_max:          %.2f m (%.2f km)\n", altura, altura / 1000.0);
    printf("  - Ganancia en vuelo libre:      %.2f m (%.2f km)\n", resultados.alturaAdicional, resultados.alturaAdicional / 1000.0);
    printf("--------------------------------------------------------------\n");

    // Pausa interactiva antes de generar el resumen definitivo
    if (!preguntar_continuacion("Resumen de Mision (Informe final y registro)"))
    {
        return cantidadSimulaciones;
    }

    // Guardar en el historial
    historial[cantidadSimulaciones].parametros = parametros;
    historial[cantidadSimulaciones].resultados = resultados;
    cantidadSimulaciones = cantidadSimulaciones + 1;

    // Pantalla de resumen final
    system("cls");
    printf("==============================================================\n");
    printf("             RESUMEN DE MISION - ARTEMIS 1                    \n");
    printf("==============================================================\n");
    mostrar_resultados(resultados);
    printf("\n--------------------------------------------------------------\n");
    printf("Simulacion registrada con exito en el historial (%d de %d).\n",
           cantidadSimulaciones, capacidad_historial);

    esperar_tecla();

    return cantidadSimulaciones;
}

// Muestra la configuracion de parametros de entrada de una simulacion
void mostrar_parametros(Parametros parametros)
{
    printf("Aceleracion fase 1:      %.2f m/s2\n", parametros.aceleracion1);
    printf("Tiempo de separacion:    %.2f s (%.2f min)\n",
           parametros.tiempoSeparacion, parametros.tiempoSeparacion / 60.0);
    printf("Aceleracion fase 2:      %.2f m/s2\n", parametros.aceleracion2);
    printf("Altura de apagado:       %.2f m (%.2f km)\n",
           parametros.alturaApagado, parametros.alturaApagado / 1000.0);
    printf("Intervalo de simulacion: %.2f s\n", parametros.intervaloSimulacion);
}

// Muestra todos los resultados fisicos de la simulacion
void mostrar_resultados(Resultados resultados)
{
    int minutos;
    double segundosRestantes;

    printf("\na) Velocidad al separar SRB:      %10.2f m/s  (%8.2f km/h)\n",
           resultados.velocidad1, resultados.velocidad1 * 3.6);
    printf("   Altura al separar SRB:         %10.2f m    (%8.2f km)\n",
           resultados.altura1, resultados.altura1 / 1000.0);

    minutos = (int)(resultados.tiempo2 / 60.0);
    segundosRestantes = resultados.tiempo2 - minutos * 60.0;
    printf("\nb) Velocidad al apagado:          %10.2f m/s  (%8.2f km/h)\n",
           resultados.velocidad2, resultados.velocidad2 * 3.6);
    printf("   Tiempo de la fase 2:           %10.2f s    (%d min %.2f s)\n",
           resultados.tiempo2, minutos, segundosRestantes);

    printf("\nc) Altura adicional (vuelo libre): %10.2f m    (%8.2f km)\n",
           resultados.alturaAdicional, resultados.alturaAdicional / 1000.0);
    printf("   Altura maxima (apogeo):        %10.2f m    (%8.2f km)\n",
           resultados.alturaMaxima, resultados.alturaMaxima / 1000.0);

    minutos = (int)(resultados.tiempoTotal / 60.0);
    segundosRestantes = resultados.tiempoTotal - minutos * 60.0;
    printf("\n   Tiempo total hasta apogeo:     %10.2f s    (%d min %.2f s)\n",
           resultados.tiempoTotal, minutos, segundosRestantes);

    printf("\nd) Aceleracion fase 1:            %10.2f G\n",
           resultados.multiploGravedad);
}

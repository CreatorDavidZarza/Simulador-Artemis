#include <stdio.h>
#include <stdlib.h>
#include "validaciones.h"


int leer_entero(int minimo, int maximo)
{
    int numero;
    char linea[100];
    char caracterExtra;
    int lecturaCorrecta;
    lecturaCorrecta = 0;
    int i;
    int encontroSalto;
    while (lecturaCorrecta == 0)
    {
        printf("ingrese un numero entero: ");
        if (fgets(linea, 100, stdin) != NULL)
        {
            i = 0;
            encontroSalto = 0;
            while (linea[i]!= '\0')
            {
               if(linea[i] == '\n')
               {
                   encontroSalto = 1;
               }
               i++;
            }
            if(encontroSalto == 0)
            {
                while ((i = fgetc(stdin))!= '\n' && i != EOF)
                {

                }
                printf("Error: la entrada es demasiado larga. \n");
                continue;
            }
            if (sscanf(linea, "%d %c", &numero, &caracterExtra) == 1)
            {
                if (numero >= minimo && numero <= maximo)
                {
                    lecturaCorrecta = 1;                
                }
                else 
                {
                    printf("Error: el numero ingresado debe estar entre %d y %d. \n", minimo, maximo);
                }
            }
            else
            {
                printf("Error: debe ingresar un numero entero valido. ");
            }

        }
        else 
        {
            printf("Error al leer la entrada. Intente nuevamente.\n");
            return EOF;
        }
    }
    return numero;
}

double leer_real(double minimo, double maximo)
{
    double numero;
    char linea[100];
    char caracterExtra;
    int lecturaCorrecta;
    int i;
    int encontroSalto;

    lecturaCorrecta = 0;

    while (lecturaCorrecta == 0)
    {
        printf("Ingrese un numero real: ");

        if (fgets(linea, 100, stdin) != NULL)
        {
            i = 0;
            encontroSalto = 0;

            while (linea[i] != '\0')
            {
                if (linea[i] == '\n')
                {
                    encontroSalto = 1;
                }
                i++;
            }

            if (encontroSalto == 0)
            {
                while ((i = fgetc(stdin)) != '\n' && i != EOF)
                {

                }

                printf("Error: la entrada es demasiado larga.\n");
                continue;
            }

            if (sscanf(linea, "%lf %c", &numero, &caracterExtra) == 1)
            {
                if (numero >= minimo && numero <= maximo)
                {
                    lecturaCorrecta = 1;
                }
                else
                {
                    printf("Error: el numero debe estar entre %.2f y %.2f.\n", minimo, maximo);
                }
            }
            else
            {
                printf("Error: debe ingresar un numero real valido.\n");
            }
        }
        else
        {
            printf("Error al leer la entrada.\n");
            return 0;
        }
    }

    return numero;
}

//VALIDACION DE LA ESTRUCTURA DE PARAMETROS

int validar_parametros(Parametros parametros)
{
    double altura1; 
   
    if (parametros.aceleracion1 <= 0)
    {
        return 0;
    }

    if (parametros.tiempoSeparacion <= 0)
    {
        return 0;
    }

    altura1 = (parametros.aceleracion1 * parametros.tiempoSeparacion * parametros.tiempoSeparacion) / 2.0;


    if (parametros.aceleracion2 <= 0)
    {
        return 0;
    }

    if (parametros.alturaApagado <= altura1)
    {
        return 0;
    }

    if (parametros.intervaloSimulacion < intervalo_minimo || parametros.intervaloSimulacion > intervalo_maximo)
    {
        return 0;
    }

    return 1;
}

void mostrar_error(void)
{
    printf("\nError: los parametros ingresados no son validos. \n");
    printf("Revise los valores e intente nuevamente \n");
}
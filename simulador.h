#ifndef SIMULADOR_H
#define SIMULADOR_H



#define gravedad 9.80 //en el libro de C joyanes #define crea constantes simbolicas y no lleva ; 
#define velocidadInicial 0.0
#define alturaInicial 0.0


//Constantes del programa

#define capacidad_historial 5
#define intervalo_minimo 1.0
#define intervalo_maximo 10.0

//Constantes de la actividad original 
#define aceleracion1_original 20.0
#define tiempo_separacion_original 120.0
#define aceleracion2_original 12.5
#define altura_apagado_original 160000.0
#define intervalo_original 1.0

//Estructura de parametros  

typedef struct {
    double aceleracion1; //double almacena valores numericos con decimales
    double tiempoSeparacion;
    double aceleracion2;
    double alturaApagado;
    double intervaloSimulacion;
} Parametros;

typedef struct {
    double velocidad1;
    double altura1;
    double tiempo2;
    double velocidad2;
    double alturaAdicional;
    double alturaMaxima;
    double tiempoTotal;
    double multiploGravedad;
} Resultados;

typedef struct { 
    Parametros parametros;
    Resultados resultados;
} Simulacion;  //agrupa los parametros utilizados y los resultados obtenidos en una misma simulacion 


//PROTOTIPOS DE FUNCIONES 

//CONFIGURACION DE PARAMETROS

Parametros cargar_parametros_originales(void); //devuelve la configuracion original del escenario
Parametros modificar_parametros(Parametros parametros); //recibe la configuracion actual y devuelve la modificacion 

//SIMULACION 
int ejecutar_simulacion(Parametros parametros, Simulacion historial[],int cantidadSimulaciones); //ejecuta una mision y devuelve la cantidad actualizada de simulaciones realizadas

//HISTORIAL
void consultar_historial(Simulacion historial[], int cantidadSimulaciones); //permite consultar simulaciones anteriores
void comparar_simulaciones(Simulacion historial[], int cantidadSimulaciones); //permite comparar diferentes simulaciones

//void no devuelve ningun valor, int devuelve un valor entero, parametros devuelve una estrucutra de tipo Parametros.


#endif 

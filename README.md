# 🚀 Simulador de Misiones Artemis – Escenario 1 (SLS)

Sistema desarrollado en **lenguaje C**. Simula el ascenso del cohete SLS (Space Launch System) durante las primeras etapas de la misión Artemis, aplicando conceptos de física y movimiento rectilíneo uniformemente variado (MRUV).

El programa no muestra una representación gráfica del cohete. La simulación representa matemáticamente su movimiento y permite observar cómo avanza durante el tiempo mediante valores numéricos de altura, distancia y velocidad.


**VIDEO EXPLICATIVO GRUPAL (Todo el código en C):**


---

## ▶️ ¿Cómo ejecutar el programa?

El proyecto incluye un archivo ejecutable (`Simulador-Artemis.exe`) para facilitar su utilización en Windows, sin necesidad de instalar un compilador de C.

**Video explicativo de cómo descargar el EJECUTABLE .EXE:**
https://youtu.be/-oIm6Hfa0Ao

**Video demo del funcionamiento del simulador:**
https://youtu.be/cRmX9F54yUk


## 👥 Integrantes – Grupo D4

- Arce Serial, Mariano Gabriel
- Centurion Schneider, André
- Kulman, Alex Richart
- Zarza, Matías David

---

# 📚 Comisión

**Comisión:** D
---

# 📐 Ecuaciones físicas implementadas

Para representar el movimiento del cohete se utilizaron ecuaciones de **Movimiento Rectilíneo Uniformemente Variado (MRUV)** y fórmulas de movimiento vertical bajo la acción de la gravedad.

### 1. Primera fase – Ascenso con aceleración constante

Se calcula la velocidad y la altura alcanzada hasta el momento de separación de los propulsores sólidos SRB.

**Velocidad final:**

**vf = v₀ + a × t**

**Altura alcanzada:**

**hf = h₀ + v₀ × t + ½ × a × t²**

**Uso:** Determinar la velocidad y altura del cohete al finalizar la primera fase y separar los propulsores SRB.

### 2. Segunda fase – Continuación de la propulsión

Se calcula la velocidad al alcanzar la altura de apagado de los motores.

**Velocidad final:**

**vf = √(v₀² + 2 × a × Δh)**

**Tiempo de la fase:**

**t = (vf − v₀) / a**

**Uso:** Calcular la velocidad y el tiempo necesarios para alcanzar la altura configurada de apagado de motores.

### 3. Tercera fase – Vuelo libre

Una vez apagados los motores, el cohete continúa ascendiendo por inercia mientras la gravedad reduce progresivamente su velocidad.

**Altura adicional:**

**Δh = v₀² / (2 × g)**

**Tiempo hasta alcanzar la altura máxima:**

**t = v₀ / g**

**Altura máxima:**

**hmáx = hapagado + Δh**

**Uso:** Calcular cuánto asciende el cohete después del apagado de motores, cuánto tiempo tarda en alcanzar el apogeo y cuál es su altura máxima.

**Dato utilizado:** La aceleración de la gravedad se considera constante, con un valor de **g = 9,80 m/s²**.

# 🛰️ ¿Qué simula el programa?

El simulador representa el ascenso del cohete SLS mediante tres fases:

1. **Primera fase:** Ascenso inicial con aceleración constante y propulsión de los motores principales y los propulsores sólidos SRB.
2. **Separación de SRB:** Se representa el momento en que los propulsores sólidos se desprenden.
3. **Segunda fase:** Continuación del ascenso hasta alcanzar la altura establecida para el apagado de motores.
4. **Fase de vuelo libre:** El cohete continúa ascendiendo por inercia, bajo la acción de la gravedad, hasta alcanzar su altura máxima.

Durante la simulación se muestran los valores correspondientes a la evolución del movimiento.

## Magnitudes calculadas

- Tiempo transcurrido (segundos).
- Altura alcanzada (metros y kilómetros).
- Velocidad (metros por segundo y kilómetros por hora).
- Aceleración.
- Altura máxima alcanzada.
- Tiempo total de ascenso.

---

# ⚙️ Funcionalidades

El programa cuenta con un menú principal que permite:

1. **Nueva simulación:** Ejecutar una nueva simulación del ascenso del cohete.
2. **Modificar parámetros:** Cambiar las condiciones iniciales y los valores utilizados en la simulación.
3. **Consultar historial:** Visualizar las simulaciones realizadas y sus resultados.
4. **Comparar simulaciones:** Comparar los parámetros y resultados de dos simulaciones diferentes.
5. **Salir:** Finalizar el programa.

El historial permite almacenar hasta **5 simulaciones** durante la ejecución del programa.

---

# 📂 Estructura del proyecto

| Archivo | Descripción |
|---------|-------------|
| `main.c` | Menú principal y control general del programa. |
| `simulador.c` | Desarrollo de la simulación física y cálculo de resultados. |
| `simulador.h` | Declaración de estructuras, constantes y prototipos del simulador. |
| `historial.c` | Consulta y comparación de simulaciones realizadas. |
| `historial.h` | Declaración de las funciones del historial. |
| `validaciones.c` | Funciones para validar los datos ingresados por el usuario. |
| `validaciones.h` | Declaración de las funciones de validación. |

---

# 🛠️ Tecnologías utilizadas

- Lenguaje C.
- GCC (compilador).
- Git.
- GitHub.
- Visual Studio Code.

---

# 📚 Contenidos aplicados

El desarrollo del proyecto permitió aplicar los contenidos trabajados durante la cursada:

- Variables y constantes.
- Estructuras condicionales.
- Ciclos de repetición.
- Funciones y modularización.
- Parámetros y valores de retorno.
- Estructuras (`struct`).
- Arreglos.
- Manejo de punteros y parámetros de funciones.
- Validación de datos.
- Simulación de procesos.
- Cálculos matemáticos y físicos.
- Manejo de errores.

---


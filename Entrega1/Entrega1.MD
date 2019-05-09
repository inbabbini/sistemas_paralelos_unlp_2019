# Entrega 1

### Ejercicio 1)

**Notas**:
* Inicializar ABCD por separado? (reducir cache overhead) 
NO TIENE SENTIDO, NO SE CUENTA

* Guardar B D por columnas? mejoraria la multiplicacion por acceso por columnas
SI

* Objetivo de analizar tiempo de AA (paralelo? secuencial? ambos?) pareciera crecer como el resto, incluso tardar mas
COMPARAR A + A TRASNPUESTA (contar tiempo de transponer) con ACCEDER A FILA y COLUMNA

* paralelizar calculo de cada matriz individual AA AB CD, o paralelizar calculo de cada elemento de la matriz?
POR ELEMENTO, comparar 3 mul juntas con por separado


### Ejercicio 2)

**Notas**:
* Mover al liberación del espacio a los threads? 
NO SE CUENTA

* Hacer la sumatoria fuera de los threads? Todas de una 
SI, SUMAR LOCALMENTE EN PARCIAL Y SUMAR PARCIALES (PARALELIZAR SUMA POR CHUNKS)

* La condicion del for no funciona, necesita que matrices/threads sea redondo

* Hacer q cada thread agarre mtx/cores o procesar una matrix por thread? (join de a lo sumo 4 procesos hasta q no haya más matrices) 

* Como parametrizar cantidad de matrices (solo para ej2p)
CAMBIAR array[] POR double**


**Notas Generales**

* Cuantos resultados usar para calcular T, y subsecuentemente Speedup y Eficiencia
CON 1 esta bien

* Desde donde tomo el tiempo? incluir alloc y free? 
CONTAR SOLO COMUPUTO

* Hay muestra?
eventualmente

* puedo correr en otra PC con 4 nucleos?
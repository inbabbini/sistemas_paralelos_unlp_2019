# Entrega 1

### Ejercicio 1)

**Notas**:
* Inicializar ABCD por separado? (reducir cache overhead) 
NO TIENE SENTIDO, NO SE CUENTA

* Guardar B D por columnas? mejoraria la multiplicacion por acceso por columnas **DONE**
SI

* Objetivo de analizar tiempo de AA (paralelo? secuencial? ambos?) pareciera crecer como el resto, incluso tardar mas **DONE: generado transp_test.c para comparar AxA vs AxTA**
COMPARAR A + A TRASNPUESTA (contar tiempo de transponer) con ACCEDER A FILA y COLUMNA **DONE**

* paralelizar calculo de cada matriz individual AA AB CD, o paralelizar calculo de cada elemento de la matriz?
POR ELEMENTO, comparar 3 mul juntas con por separado **ej_s_join_mul.c** **DONE**


### Ejercicio 2)

**Notas**:
* Hacer la sumatoria fuera de los threads? Todas de una 
SI, SUMAR LOCALMENTE EN PARCIAL Y SUMAR PARCIALES (PARALELIZAR SUMA POR CHUNKS)

* La condicion del for no funciona, necesita que matrices/threads sea redondo
**DONE: usar numeros redondos**

* Hacer q cada thread agarre mtx/cores o procesar una matrix por thread? (join de a lo sumo 4 procesos hasta q no haya más matrices) 
CADA PROC AGARRA VARIAS MATRICES **DONE**

* Como parametrizar cantidad de matrices (solo para ej2p)
CAMBIAR array[] POR double** **DONE**


**Notas Generales**

* Cuantos resultados usar para calcular T, y subsecuentemente Speedup y Eficiencia
CON 1 esta bien

* Desde donde tomo el tiempo? incluir alloc y free? 
CONTAR SOLO COMUPUTO

* Hay muestra?
eventualmente

* puedo correr en otra PC con 4 nucleos?
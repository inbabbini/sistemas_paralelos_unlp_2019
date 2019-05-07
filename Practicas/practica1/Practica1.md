#Practica1

### 1-Analizar el algoritmo matrices.c que resuelve la multiplicación de matrices cuadradas de NxN. ¿Dónde cree que se producen demoras? ¿Cómo podría optimizarse el código? Implementar una solución optimizada y comparar los tiempos probando con diferentes tamaños de matrices.

Hay varias practicas que, si bien aportan legibilidad, también agregan overhead, tales como:

- Uso intensivo de procedimientos y funciones, los cuales generan overhead por el uso de la stack y el constante cambio de contexto

- 
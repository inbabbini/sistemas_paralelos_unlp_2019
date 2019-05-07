/*
Usando OpenMP, resolver la expresion: AA + AB + CD

Donde A, B, C y D son matrices de NxN.
Evaluar para N=512, 1024 y 2048.
Analizar que ocurre con el producto AA.
Analizar que ocurre con los tiempos de ejecución al tratar las multiplicaciones
de matrices individualmente con respecto a tratarlas en conjunto.
*/
#include<stdio.h>
#include <sys/time.h>

/**********Para calcular tiempo***********************************/
double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
/****************************************************************/


int main(int argc, char const *argv[])
{
    return 0;
}

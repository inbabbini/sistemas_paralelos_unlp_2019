/*
Usando PThreads resolver la expresion:

m
∑ ((maxMi − minMi) / avgMi))⋅Mi
i=0

Donde Mi son matrices cuadradas de NxN.
minMi y maxMi son el mínimo y el máximo valor de los elementos de la matriz Mi, respectivamente.
AvgMi es el valor promedio de los elementos de la matriz Mi.
Evaluar para N=512, 1024 y 2048 y m lo suficiente grande para que el tiempo del algoritmo paralelo supere el tiempo del algoritmo secuencial
*/

#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#define DEBUG_MODE 0

//Globals
int N = 512;
int matrixCount = 5;

double **matrixes;
double *total;

//Functions
double dwalltime();
void printFirstRow(double *matrix, char *name);


int main(int argc, char const *argv[])
{
    int m, i, j, high, low;
    double maxM, minM, avgM, e, timetick;
    high = 100;
    low = 0;

    if ((argc != 3) || 
        ((N = atoi(argv[1])) <= 0) || 
        ((matrixCount = atoi(argv[2])) <= 0))
    {
        printf("\nUsar: %s n matrixCount\nn: Dimension de la matriz (nxn X nxn)\nmatrixCount: cantidad de matrices\n", argv[0]);
        exit(1);
    }

    printf("\n---Process started---\nMatrix Count: %i\n", matrixCount);

    //Declare and alloc Total
    total = (double*)malloc(sizeof(double)*N*N);
    //Initialize Total
    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            total[i*N+j] = 0;
        }
    }

    //alloc matrixes
    matrixes = (double**)malloc(sizeof(double*) * matrixCount);
    for (int i = 0; i < matrixCount; i++)
    {
        matrixes[i] = (double*)malloc(sizeof(double)*N*N);
    }

    for (int m = 0; m < matrixCount; m++)
    {
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                matrixes[m][i*N+j] = ((double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
            }
        }
    }

    //Initialize array of matrixes with random numbers
    
    
    //Start taking time
    printf("Starting calculations\n");
    timetick = dwalltime();

    for(int m = 0; m < matrixCount; m++)
    {
        maxM = 0;
        minM = DBL_MAX;
        avgM = 0;

        for(i=0;i<N;i++){
            for(j=0;j<N;j++)
            {
                //Find Max
                if(matrixes[m][i*N+j] > maxM)
                {
                    maxM = matrixes[m][i*N+j];
                } 
                else if(matrixes[m][i*N+j] < minM)
                {
                    minM = matrixes[m][i*N+j];
                }

                //sum Avg
                avgM = avgM + matrixes[m][i*N+j];
            }
        }

        //finalize AVG
        avgM = avgM / (N*N);

        //calculate e = (maxMi - minMi)/avgMi
        e = (maxM - minM) / avgM;

        //Print Matrix Data
        if(DEBUG_MODE){printf("Matrix %i: maxM = %f | minM = %f | avgM = %f | e = %f \n", m, maxM, minM, avgM, e);}
        if(DEBUG_MODE){printFirstRow(matrixes[m], "Random");}

        //calculate e * Mi, then add to Total matrix
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                total[i*N+j] = total[i*N+j] + (matrixes[m][i*N+j] * e);
            }
        }
    }

    //Print some results
    if(DEBUG_MODE){printFirstRow(total, "Total");}
    
    //Print process time
    printf("Time in seconds: %f\n", dwalltime() - timetick);
    
    //FreeMemory
    for(int m = 0; m < matrixCount; m++)
    {
        free(matrixes[m]);
    }
    free(total);
    free(matrixes);

    return 0;
}

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

void printFirstRow(double *matrix, char *name)
{   
    printf("Matrix: %s\n [", name);
    for(int i = 0; i < N; i++)
    {
        printf("%f,", matrix[i]);
    }
    printf("]\n\n");
    
}
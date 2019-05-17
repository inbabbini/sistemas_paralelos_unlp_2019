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

//Globals
int N = 512;
int matrixCount = 5;

//Functions
double dwalltime();
double findMax(double *matrix);
double findMin(double *matrix);
double findAvg(double *matrix);
void multiplyMatrix(double *matrix, double scalar);
void addMatrixBtoA(double *A, double *B);
void printFirstRow(double *matrix, char *name);


int main(int argc, char const *argv[])
{
    int m, i, j, high, low;
    double maxM, minM, avgM, e, timetick;

    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((matrixCount = atoi(argv[2])) <= 0))
    {
        printf("\nUsar: %s n matrixCount\n  n: Dimension de la matriz (nxn X nxn)\n matrixCount: cantidad de matrices\n", argv[0]);
        exit(1);
    }

    timetick = dwalltime();

    //Declare and alloc Total
    double *total;
    total = (double*)malloc(sizeof(double)*N*N);
    //Initialize Total
    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            total[i*N+j] = 0;
        }
    }

    //Declare and alloc array of matrixes
    double *array[matrixCount];
    for(int i = 0; i < matrixCount; i++)
    {
        array[i] = (double*)malloc(sizeof(double)*N*N);
    }

    //Array accessor
    double *(*p)[] = &array;

    //Initialize array of matrixes with random numbers
    high = 100;
    low = 0;
    for(int m = 0; m < matrixCount; m++)
    {
        for(i=0;i<N;i++){
            for(j=0;j<N;j++)
            {
                (*p)[m][i*N+j] = ((double)rand() * ( high - low ) ) / (double)RAND_MAX + low;
            }
        }

        printFirstRow((*p)[m], ("randm"+m));
    }

    for(int m = 0; m < matrixCount; m++)
    {
        //find maxMi
        maxM = findMax((*p)[m]);
        //find minMi
        minM = findMin((*p)[m]);
        //find avgMi
        avgM = findAvg((*p)[m]);

        //calculate e = (maxMi - minMi)/avgMi
        e = (maxM - minM) / avgM;

        printf("\n Matrix %i: maxM = %f | minM = %f | avgM = %f | e = %f \n", m, maxM, minM, avgM, e);

        //calculate e * Mi
        multiplyMatrix((*p)[m], e);

        //add Mi to total
        addMatrixBtoA(total,(*p)[m]);        
    }

    //Print some results
    printFirstRow(total, "total");
    

    printf("Tiempo en segundos %f \n", dwalltime() - timetick);
    
    for(int m = 0; m < matrixCount; m++)
    {
        free((*p)[m]);
    }
    free(total);


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

double findMax(double *matrix)
{
    int i, j;
    int max = 0;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            if (matrix[i*N+j] > max) {
                max = matrix[i*N+j];
            }
        }
    }
    
    return max;
}

double findMin(double *matrix)
{
    int i, j;
    double min = DBL_MAX;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            if (matrix[i*N+j] < min) {
                min = matrix[i*N+j];
            }
        }
    }

    return min;
}

double findAvg(double *matrix)
{
    int i, j;
    double total = 0;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            total = total + matrix[i*N+j];
        }
    }

    return total/(N*N);
}

void multiplyMatrix(double *matrix, double scalar)
{
    int i, j;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            matrix[i*N+j] = matrix[i*N+j] * scalar;
        }
    }
}

void addMatrixBtoA(double *A, double *B)
{
    int i, j;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            A[i*N+j] = A[i*N+j] + B[i*N+j];
        }
    }
}

void printFirstRow(double *matrix, char *name)
{   
    printf("Matrix: %s\n [", name);
    for(int i = 0; i < N; i++)
    {
        printf("%f,", matrix[i]);
    }
    printf("]\n\n");
    
}
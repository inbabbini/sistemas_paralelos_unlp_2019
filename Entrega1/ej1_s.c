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
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <omp.h>



//Globals
int N = 512;
int thread_count = 2;
int DEBUG_MODE = 0;

//Functions
double dwalltime();
void printFirstRow();

int main(int argc, char const *argv[])
{
    int i,j,k;
    double timetick, pm_timetick;

    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((thread_count = atoi(argv[1])) <= 0))
    {
        printf("\nUsar: %s N n_threads\n  N: Dimension de la matriz (NxN)\n n_threads: Cantidad de threads", argv[0]);
        exit(1);
    }

    //Declare and alloc matrices
    double *A, *B, *C, *D, *AA, *AB, *CD, *AAABCD;

    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    AA=(double*)malloc(sizeof(double)*N*N);
    AB=(double*)malloc(sizeof(double)*N*N);
    CD=(double*)malloc(sizeof(double)*N*N);
    AAABCD=(double*)malloc(sizeof(double)*N*N);

    //Initialize matrixes A, B, C, D
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            A[i*N+j]=1;
            B[i+j*N]=1;
            C[i*N+j]=1;
            D[i*N+j]=1;
        }
    }

    //Start taking time
    timetick = dwalltime();
    printf("Starting calculations\n");


    //Process AA = AxA
    pm_timetick = dwalltime();
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            AA[i*N+j]=0;
            for(k=0;k<N;k++){
                AA[i*N+j]= AA[i*N+j] + A[i*N+k]*A[k+j*N];
            }
        }
    }
    printf("Matrix %s took time in seconds: %f \n", "AA", dwalltime() - pm_timetick);

    //Process AB = AxB
    pm_timetick = dwalltime();
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            AB[i*N+j]=0;
            for(k=0;k<N;k++){
                AB[i*N+j]= AB[i*N+j] + A[i*N+k]*B[k+j*N];
            }
        }
    }
    printf("Matrix %s took time in seconds: %f \n", "AB", dwalltime() - pm_timetick);

    //Process CD = CxD
    pm_timetick = dwalltime();
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            CD[i*N+j]=0;
            for(k=0;k<N;k++){
                CD[i*N+j]= CD[i*N+j] + A[i*N+k]*B[k+j*N];
            }
        }
    }
    printf("Matrix %s took time in seconds: %f \n", "CD", dwalltime() - pm_timetick);

    //Process AAABCD
    pm_timetick = dwalltime();
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            AAABCD[i*N+j]= AA[i*N+j] + AB[i*N+j] + CD[i*N+j];
        }
    }
    printf("Matrix %s took time in seconds: %f \n", "AAABCD", dwalltime() - pm_timetick);

    //Print Total time
    printf("Total process took time in seconds: %f \n", dwalltime() - timetick);

    //Check results
    int check = 1;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            check=check&&(AAABCD[i*N+j]==(N*3.00));
        }
    }

    if(check)
    {
        printf("Processed values verified OK\n");
    }
    else
    {
        printf("Processed values have errors\n");
    }

    if(DEBUG_MODE)
    {
        printFirstRow(A,"A");
        printFirstRow(B,"B");
        printFirstRow(C,"C");
        printFirstRow(D,"D");
        printFirstRow(AA,"AA");
        printFirstRow(AB,"AB");
        printFirstRow(CD,"CD");
        printFirstRow(AAABCD,"AAABCD");
    }

    //Free space
    free(A);
    free(B);
    free(C);
    free(D);
    free(AA);
    free(AB);
    free(CD);
    free(AAABCD);

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
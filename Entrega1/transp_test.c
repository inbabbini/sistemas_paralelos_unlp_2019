#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <omp.h>

//Globals
int N = 5;
int thread_count = 2;
int DEBUG_MODE = 0;

//Functions
double dwalltime();
void printMatrix(double *matrix, char *name);

int main(int argc, char const *argv[])
{
    int i,j,k;
    double timetick, pm_timetick;

    if ((argc != 2) || ((N = atoi(argv[1])) <= 0))
    {
        printf("\nUsar: %s N n_threads\n  N: Dimension de la matriz (NxN)\n n_threads: Cantidad de threads", argv[0]);
        exit(1);
    }

    //Declare and alloc matrices
    double *A, *TA, *AA, *ATA;

    A=(double*)malloc(sizeof(double)*N*N);
    TA=(double*)malloc(sizeof(double)*N*N);
    AA=(double*)malloc(sizeof(double)*N*N);
    ATA=(double*)malloc(sizeof(double)*N*N);


    //Initialize matrixes A
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            A[i*N+j]=i;
        }
    }
    printMatrix(A, "A");

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
    printMatrix(AA, "AA");

    //TRANSPOSE A for better access time
    pm_timetick = dwalltime();
    for (int i = 0; i < N; i++)
    {
        TA[i*N+i] = A[i*N+i];
        for (int j = i + 1; j < N; j++)
        {
            TA[i*N+j] = A[j*N+i];
            TA[j*N+i] = A[i*N+j];
        }
    }
    printf("Matrix %s transposing took time in seconds: %f \n", "A", dwalltime() - pm_timetick);
    printMatrix(TA, "TA");

    pm_timetick = dwalltime();
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            ATA[i*N+j]=0;
            for(k=0;k<N;k++){
                ATA[i*N+j]= ATA[i*N+j] + A[i*N+k]*TA[i*N+k];
            }
        }
    }
    printf("Matrix %s took time in seconds: %f \n", "ATA", dwalltime() - pm_timetick);
    printMatrix(ATA, "ATA");

    free(A);
    free(TA);
    free(AA);
    free(ATA);

    exit(0);
}

void printMatrix(double *matrix, char *name)
{
    if(DEBUG_MODE)
    {
        printf("Matrix %s\n", name);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%f ", matrix[i*N+j]);
            }
            printf("\n");
        }
    }
}

double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}
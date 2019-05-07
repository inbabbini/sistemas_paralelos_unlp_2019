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
#include <pthread.h>

//Constants
#define DEBUG_MODE 1
#define THREAD_COUNT 4
#define DEFAULT_N 512
#define DEFAULT_MATRIXCOUNT 5

//Globals
int N = DEFAULT_N;
int matrixCount = DEFAULT_MATRIXCOUNT;
int chunk_size;
double *array[DEFAULT_MATRIXCOUNT];

int rand_low = 0;
int rand_high = 100;

double *totalMatrix;
pthread_mutex_t add_matrix_lock;

//Functions
double dwalltime();
void printFirstRow(double *matrix, char *name);
void *processAndAddMatrix(void *matrix_ptr);
void *processAndAddMatrixes(void *task_id);
void debugP(char *string);


int main(int argc, char const *argv[])
{
    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((matrixCount = atoi(argv[2])) <= 0))
    {
        printf("\nUsar: %s n matrixCount\n  n: Dimension de la matriz (nxn X nxn)\n matrixCount: cantidad de matrices\n", argv[0]);
        exit(1);
    }

    int thread_id, thread_ids[THREAD_COUNT];
    pthread_attr_t attr;
    pthread_t threads[THREAD_COUNT];
    pthread_attr_init(&attr);

    chunk_size = (matrixCount/THREAD_COUNT);
    printf("Chunk size: %i\n", chunk_size);

    int i, j;
    double timetick;

    //Init mutex for adding results to totalMatrix
    pthread_mutex_init(&add_matrix_lock, NULL);

    timetick = dwalltime();

    //Declare and alloc totalMatrix
    totalMatrix = (double*)malloc(sizeof(double)*N*N);

    //Initialize totalMatrix
    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            totalMatrix[i*N+j] = 0;
        }
    }
    
    printFirstRow(totalMatrix, "totalMatrix before process");

    //Declare and alloc array of matrixes
    //double *array[matrixCount]; THIS FOR DYNAMIC matrix count

    debugP("allocing memory for matrices");
    printf("Matrix count: %i", matrixCount);

    for(int i = 0; i < matrixCount; i++)
    {
        array[i] = (double*)malloc(sizeof(double)*N*N);
    }

    //Array accessor
    double *(*p)[] = &array;

    debugP("Launching threads");
    
    //Launch Threads
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        printf("Launching thread %i\n", i);

        thread_ids[i] = i;
        pthread_create(
            &threads[i],
            &attr,
            processAndAddMatrixes,
            &thread_ids[i]
        );
    }
    
    debugP("Waiting threads to end");

    //Wait for threads to end
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
        printf("thread %i ended\n", i);
    }

    debugP("Printing results");

    //Print some results
    printFirstRow(totalMatrix, "totalMatrix");
    
    printf("Tiempo en segundos %f \n", dwalltime() - timetick);
    
    //FreeMemory
    for(int m = 0; m < matrixCount; m++)
    {
        free((*p)[m]);
    }
    free(totalMatrix);

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

void debugP(char *string)
{
    if(DEBUG_MODE)
    {
        printf("\n\n**********\nDEBUG: %s\n**********\n\n", string);
    }
}

void *processAndAddMatrix(void *matrix_ptr)
{
    int m, i, j;
    double maxM, minM, avgM, e;
    double *matrix = (double *) matrix_ptr;

    maxM = 0;
    minM = DBL_MAX;
    avgM = 0;

    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
        {
            //Assign random value
            matrix[i*N+j] = ((double)rand() * ( rand_high - rand_low ) ) / (double)RAND_MAX + rand_low;

            //Find Max
            if(matrix[i*N+j] > maxM)
            {
                maxM = matrix[i*N+j];
            } 
            else if(matrix[i*N+j] < minM)
            {
                minM = matrix[i*N+j];
            }

            //sum Avg
            avgM = avgM + matrix[i*N+j];
        }
    }

    //finalize AVG
    avgM = avgM / (N*N);

    //calculate e = (maxMi - minMi)/avgMi
    e = (maxM - minM) / avgM;

    //Print Matrix Data
    printf("\n Matrix %i: maxM = %f | minM = %f | avgM = %f | e = %f \n", m, maxM, minM, avgM, e);
    printFirstRow(matrix, "Random");

    //Start Critical Region
    pthread_mutex_lock(&add_matrix_lock);

    //calculate e * Mi, then add to totalMatrix
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            totalMatrix[i*N+j] = totalMatrix[i*N+j] + (matrix[i*N+j] * e);
        }
    }
    //End Critial Region
    pthread_mutex_unlock(&add_matrix_lock);
}

void *processAndAddMatrixes(void *task_id_ptr)
{
    int *ptr, task_id;
    ptr = (int*) task_id_ptr;
    task_id = *ptr;

    printf("Starting thread %i\n", task_id);
    
    int m, i, j;
    double maxM, minM, avgM, e;

    maxM = 0;
    minM = DBL_MAX;
    avgM = 0;

    //Array accessor
    double *(*p)[] = &array;
    
    //Work on each matrix
    //for(int m = task_id * chunk_size; (m+1) <= task_id+1 + ((task_id+1) * chunk_size); m++)
    for(int m = task_id * chunk_size; m < (task_id * chunk_size) + chunk_size; m++)    
    {
        printf("Processing matrix %i on thread %i\n", m, task_id);
        
        maxM = 0;
        minM = DBL_MAX;
        avgM = 0;

        for(i=0;i<N;i++){
            for(j=0;j<N;j++)
            {
                //Assign random value
                (*p)[m][i*N+j] = ((double)rand() * ( rand_high - rand_low ) ) / (double)RAND_MAX + rand_low;

                //Find Max
                if((*p)[m][i*N+j] > maxM)
                {
                    maxM = (*p)[m][i*N+j];
                } 
                else if((*p)[m][i*N+j] < minM)
                {
                    minM = (*p)[m][i*N+j];
                }

                //sum Avg
                avgM = avgM + (*p)[m][i*N+j];
            }
        }

        //finalize AVG
        avgM = avgM / (N*N);

        //calculate e = (maxMi - minMi)/avgMi
        e = (maxM - minM) / avgM;

        //Print Matrix Data
        printf("\n Matrix %i: maxM = %f | minM = %f | avgM = %f | e = %f \n", m, maxM, minM, avgM, e);
        //printFirstRow((*p)[m], "Random");

        //Start Critical Region
        pthread_mutex_lock(&add_matrix_lock);
        printf("Adding processed matrix %i on thread %i\n", m, task_id);
        //calculate e * Mi, then add to totalMatrix
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                totalMatrix[i*N+j] = totalMatrix[i*N+j] + ((*p)[m][i*N+j] * e);
            }
        }
        //End Critial Region
        pthread_mutex_unlock(&add_matrix_lock);
        printf("Added processed matrix %i on thread %i\n", m, task_id);
    }

    //Close thread
    pthread_exit(0);
}
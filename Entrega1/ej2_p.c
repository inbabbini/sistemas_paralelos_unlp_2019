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
#define DEBUG_MODE 0
#define DEFAULT_N 512

//Globals
int N = DEFAULT_N;
int matrixCount;
int THREAD_COUNT;
int chunk_size;

int rand_low = 0;
int rand_high = 100;

double **matrixes;
double **partialTotalMatrixes;
double *totalMatrix;
pthread_mutex_t add_matrix_lock;

//Functions
double dwalltime();
void printFirstRow(double *matrix, char *name);
void *processAndAddMatrix(void *matrix_ptr);
void *processAndAddMatrixes(void *task_id);

int main(int argc, char const *argv[])
{
    if ((argc != 4) || 
    ((N = atoi(argv[1])) <= 0) || 
    ((matrixCount = atoi(argv[2])) <= 0) ||
    ((THREAD_COUNT = atoi(argv[3]))) <= 0)
    {
        printf("\nUsar: %s n matrixCount\n n: Dimension de la matriz (nxn X nxn)\n matrixCount: cantidad de matrices\n t_count: cantidad de threads\n", argv[0]);
        exit(1);
    }

    int thread_id, thread_ids[THREAD_COUNT];
    pthread_attr_t attr;
    pthread_t threads[THREAD_COUNT];
    pthread_attr_init(&attr);
    int i, j;
    double timetick;

    chunk_size = (matrixCount/THREAD_COUNT);
    printf("\n---Process started---\nThread Count: %i\nMatrix Count: %i\nChunk size: %i\n", THREAD_COUNT, matrixCount, chunk_size);


    //Init mutex for adding results to totalMatrix
    pthread_mutex_init(&add_matrix_lock, NULL);

    //alloc partial total matrixes
    if(DEBUG_MODE) {printf("Allocing partial Total Matrixes\n");}
    partialTotalMatrixes = (double**)malloc(sizeof(double*) * THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        partialTotalMatrixes[i] = (double*)malloc(sizeof(double)*N*N);
    }

    //Initialize partial total matrixes
    if(DEBUG_MODE) {printf("Initializing partial Total Matrixes\n");}    
    for (int m = 0; m < THREAD_COUNT; m++)
    {
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                partialTotalMatrixes[m][i*N+j] = 0;
            }
        }
    }    

    //Alloc totalMatrix
    if(DEBUG_MODE) {printf("Allocing totalMatrix\n");}    
    totalMatrix = (double*)malloc(sizeof(double)*N*N);

    //Initialize totalMatrix
    if(DEBUG_MODE) {printf("Initializing totalMatrix\n");}    
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
        {
            totalMatrix[i*N+j] = 0;
        }
    }

    //alloc matrixes
    if(DEBUG_MODE) {printf("Allocing matrixes\n");}    
    matrixes = (double**)malloc(sizeof(double*) * matrixCount);
    for (int i = 0; i < matrixCount; i++)
    {
        matrixes[i] = (double*)malloc(sizeof(double)*N*N);
    }

    //Initialize matrixes
    if(DEBUG_MODE) {printf("Initializing matrixes\n");}    
    for (int m = 0; m < matrixCount; m++)
    {
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                //Assign random value
                matrixes[m][i*N+j] = ((double)rand() * ( rand_high - rand_low ) ) / (double)RAND_MAX + rand_low;
            }
        }
    }

    //Start taking time
    printf("Starting calculations\n");
    timetick = dwalltime();
    
    //Launch Threads
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        if(DEBUG_MODE){printf("Launching thread %i\n", i);}

        thread_ids[i] = i;
        pthread_create(
            &threads[i],
            &attr,
            processAndAddMatrixes,
            &thread_ids[i]
        );
    }
    
    if(DEBUG_MODE){printf("Waiting threads to end\n");}

    //Wait for threads to end
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
        if(DEBUG_MODE){printf("thread %i ended\n", i);}
    }
    
    //Print process time
    printf("Time in seconds: %f\n", dwalltime() - timetick);

    //Print some results
    if(DEBUG_MODE){printf("Printing results\n");}
    if(DEBUG_MODE){printFirstRow(totalMatrix, "totalMatrix");}
    
    
    //FreeMemory
    for(int m = 0; m < matrixCount; m++)
    {
        free(matrixes[m]);
    }
    free(matrixes);
    for(int m = 0; m < THREAD_COUNT; m++)
    {
        free(partialTotalMatrixes[m]);
    }
    free(partialTotalMatrixes);
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
    printf("\nMatrix: %s\n[", name);
    for(int i = 0; i < N; i++)
    {
        printf("%f,", matrix[i]);
    }
    printf("]\n\n");
    
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

    if(DEBUG_MODE){printf("Starting thread %i\n", task_id);}
    
    int m, i, j;
    double maxM, minM, avgM, e;

    maxM = 0;
    minM = DBL_MAX;
    avgM = 0;
    
    //Work on each matrix
    for(int m = task_id * chunk_size; m < (task_id * chunk_size) + chunk_size; m++)    
    {
        if(DEBUG_MODE){printf("Processing matrix %i on thread %i\n", m, task_id);}
        
        maxM = 0;
        minM = DBL_MAX;
        avgM = 0;

        for(i=0;i<N;i++){
            for(j=0;j<N;j++)
            {
                //Find Max and min
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

        //calculate e * Mi, then add to local partial total matrix
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                partialTotalMatrixes[task_id][i*N+j] = partialTotalMatrixes[task_id][i*N+j] + (matrixes[m][i*N+j] * e);
            }
        }
    }

    //add partialTotalMatrix to totalMatrix
    //Start Critical Region
        pthread_mutex_lock(&add_matrix_lock);
        if(DEBUG_MODE){printf("Adding processed matrix %i on thread %i\n", m, task_id);}

        //add partial matrix from thread to totalMatrix        
        for(i=0;i<N;i++)
        {
            for(j=0;j<N;j++)
            {
                totalMatrix[i*N+j] = totalMatrix[i*N+j] + partialTotalMatrixes[task_id][i*N+j];
            }
        }
        
        //End Critial Region
        pthread_mutex_unlock(&add_matrix_lock);
        if(DEBUG_MODE){printf("Added processed matrix %i on thread %i\n", m, task_id);}
    
    //Close thread
    pthread_exit(0);
}
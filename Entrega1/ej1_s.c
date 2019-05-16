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


//Globals
int N = 512;
int DEBUG_MODE = 0;
int JOINT_MUL = 0;
int USE_TRANSPOSE = 0;

//Functions
double dwalltime();
void printFirstRow();
void transpose(double* matrix);

int main(int argc, char const *argv[])
{
    int i,j,k;
    double timetick, pm_timetick; 
    
     if ((argc != 4) || 
        ((N = atoi(argv[1])) <= 0) || 
        (strcmp(argv[2], "S") != 0 && strcmp(argv[2], "J") != 0) || 
        (strcmp(argv[3], "Y") != 0 && strcmp(argv[3], "N") != 0))
    {
        printf("\n Usar: %s N: Dimension de la matriz (NxN)\n J|S: multiplicacion de matrices Juntas o Separadas\n Y|N: usar A transpuesta\n ", argv[0]);
        printf("args: %s | %s | %s\n", argv[1], argv[2], argv[3]);
        exit(1);
    }
    
    //config joint or separated multiplications
    if(strcmp(argv[3], "J") == 0) {
        JOINT_MUL = 1;
        printf("Doing Joint multiplications\n");        
    } else {
        printf("Doing Separated multiplications\n");        
    }

    //config use of transposed A matrix
    if(strcmp(argv[4], "Y") == 0){
        USE_TRANSPOSE = 1;
        printf("Using A transposed, AA = A*TA\n");
    } else {
        printf("Using regular A, AA = A*A\n");
    }

    //Declare and alloc matrices
    double *A, *TA, *B, *C, *D, *AA, *AB, *CD, *AAABCD;

    A=(double*)malloc(sizeof(double)*N*N);
    TA=(double*)malloc(sizeof(double)*N*N);
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

    if(USE_TRANSPOSE)
    {
        //TRANSPOSE A for better access time
        double v;
        for (int i = 0; i < N; i++)
        {
            TA[i*N+i] = A[i*N+i];
            for (int j = i + 1; j < N; j++)
            {
                TA[i*N+j] = A[j*N+i];
                TA[j*N+i] = A[i*N+j];
            }
        }
    }

    if(JOINT_MUL)
    {
        pm_timetick = dwalltime();
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                AA[i*N+j]=0;
                for(k=0;k<N;k++){
                    //Process AA = AxTA
                    //AA[i*N+j]= AA[i*N+j] + A[i*N+k]*TA[i*N+k];
                    if(USE_TRANSPOSE){AA[i*N+j]= AA[i*N+j] + A[i*N+k]*TA[i*N+k];} else {AA[i*N+j]= AA[i*N+j] + A[i*N+k]*A[k+j*N];}
                    //Process AB = AxB
                    AB[i*N+j]= AB[i*N+j] + A[i*N+k]*B[k+j*N];
                    //Process CD = CxD
                    CD[i*N+j]= CD[i*N+j] + C[i*N+k]*D[k+j*N];
                }
            }
        }
        printf("All matrixes (AA, AB, CD) joined took time in seconds: %f \n", dwalltime() - pm_timetick);
    }
    else
    {
        //Process AA = A x TA
        pm_timetick = dwalltime();
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                AA[i*N+j]=0;
                for(k=0;k<N;k++){
                    //AA[i*N+j]= AA[i*N+j] + A[i*N+k]*TA[i*N+k];
                    if(USE_TRANSPOSE){AA[i*N+j]= AA[i*N+j] + A[i*N+k]*TA[i*N+k];} else {AA[i*N+j]= AA[i*N+j] + A[i*N+k]*A[k+j*N];}
                }
            }
        }

        //Process AB = AxB
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                AB[i*N+j]=0;
                for(k=0;k<N;k++){
                    AB[i*N+j]= AB[i*N+j] + A[i*N+k]*B[k+j*N];
                }
            }
        }

        //Process CD = CxD
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                CD[i*N+j]=0;
                for(k=0;k<N;k++){
                    CD[i*N+j]= CD[i*N+j] + C[i*N+k]*D[k+j*N];
                }
            }
        }
        printf("All matrixes (AA, AB, CD) separeted took time in seconds: %f \n", dwalltime() - pm_timetick);        
    }

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
        if(USE_TRANSPOSE) {printFirstRow(TA,"TA");}
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
    free(TA);
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
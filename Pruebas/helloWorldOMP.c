#include <stdio.h>
#include <omp.h>

int main()
{
    int nthreads, tid;

    #pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        printf("Hello World, this is thread %i\n", tid);

        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Total number of threads: %i\n", nthreads);
        }

        printf("Closing thread: %i\n", tid);
    }

    printf("Back at secuential, finishing program");
}
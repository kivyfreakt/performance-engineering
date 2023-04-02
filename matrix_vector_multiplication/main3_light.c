
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <omp.h>
// #include <immintrin.h> // avx

#define n 8192

float matrix[n][n] __attribute__((aligned(32)));
float vector[n] __attribute__((aligned(32)));;
float result[n] __attribute__((aligned(32)));

void test_func()
{   
    // #pragma omp parallel schedule(static) num_threads()
    #pragma omp parallel
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = n/nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (n-1) : (lb + items_per_thread - 1);

        register int i;
        register int j;

        for(i = lb; i < ub; ++i)
        {
            float buff = 0;
            float *a_i = matrix[i];
            float vres[8] __attribute__((aligned(32)));

            for(j = 0; j < n; j += 8)
            {
                __asm__ __volatile__(
                    "vmovaps (%[b]), %%ymm0 \n\t"
                    "vmovaps (%[a]), %%ymm1 \n\t"
                    "vmulps %%ymm0, %%ymm1, %%ymm0 \n\t"
                    "vmovaps %%ymm0, %[_c] \n\t"
                    : [_c] "=m" (*vres)
                    : [b] "r" (vector + j), [a] "r" (a_i + j)
                    : "%ymm0", "%ymm1"
                );
                buff += vres[0] + vres[1] + vres[2] + vres[3] + vres[4] + vres[5] + vres[6] + vres[7];
            }

            result[i] = buff;
        }
    }

}

int main()
{
    // init
    for(int i = 0; i < n; i++)
    {   
        vector[i] = (float)rand() / (float)RAND_MAX;
        result[i] = 0;
        for (int j = 0; j < n; j++)
            matrix[i][j] = (float)rand() / (float)RAND_MAX;
    }

    // warmup 
    test_func();

    clock_t begin_cpu, end_cpu;
    struct timeval begin_time, end_time;


    gettimeofday(&begin_time, NULL);
    begin_cpu = clock();
    test_func();
    end_cpu = clock();
    gettimeofday(&end_time, NULL);

    long time_seconds = end_time.tv_sec - begin_time.tv_sec;
    long time_microseconds = end_time.tv_usec - begin_time.tv_usec;
    double time_elapsed = time_seconds + time_microseconds*1e-6;
    // long time_elapsed_mcs = (long)(time_elapsed*1000000 + 0.5);

    double cputime_spent = (double)(end_cpu - begin_cpu) / CLOCKS_PER_SEC;
    // long cputime_spent_mcs = (long)(cputime_spent*1000000 + 0.5);

    printf("time_elapsed: %f \ncputime_spent: %f", time_elapsed, cputime_spent);
    
    return 0;
}
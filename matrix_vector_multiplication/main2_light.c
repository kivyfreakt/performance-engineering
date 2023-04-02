
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <immintrin.h> // avx

#define n 8192

float matrix[n][n] __attribute__((aligned(32)));
float vector[n] __attribute__((aligned(32)));;
float result[n] __attribute__((aligned(32)));

void test_func()
{   
    register int i;
    register int j;

    __m256 X, Y;

    for(i = 0; i < n; ++i)
    {   
        float buff = 0;
        for(j = 0; j < n; j += 8)
        {
            X = _mm256_load_ps(&matrix[i][j]);
            Y = _mm256_load_ps(&vector[j]);
            X = _mm256_mul_ps(X, Y);
            buff += X[0] + X[1] + X[2] + X[3] + X[4] + X[5] + X[6] + X[7];
        }
        result[i] = buff;
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
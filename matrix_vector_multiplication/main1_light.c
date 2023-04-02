#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

#define n 8192

float matrix[n][n];
float vector[n];
float result[n];

void test_func()
{
    for(int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i] += matrix[i][j] * vector[j];
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
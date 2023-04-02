#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
// #include <immintrin.h> // avx

#define n 8192

float matrix[n][n] __attribute__((aligned(32)));
float vector[n] __attribute__((aligned(32)));;
float result[n] __attribute__((aligned(32)));

void test_func()
{   
    register int i;
    register int j;

    for(i = 0; i < n; ++i)
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


void check_correct()
{
    for(int i = 0; i < n; i++)
    {
        float temp = 0;
        for (int j = 0; j < n; j++)
            temp += matrix[i][j] * vector[j];
        // printf("Actual V[%d] = %-2f; Expected V[%d] = %-2f\n", i, temp, i, result[i]);
        assert(fabs(result[i] - temp) < 0.5); // float equal check....
    }
}

void print_statistics(float *measures, int m)
{
    // drop outliers with quartiles
    float q1 = 0, q3 = 0;
    for(int i = 0; i < m; i++)
    {
        for(int j = i + 1; j < m; j++)
        {
            if(measures[i] > measures[j])
            {
                float tmp = measures[i];
                measures[i] = measures[j];
                measures[j] = tmp;
            }
        }
    }
    q1 = measures[m / 4];
    q3 = measures[3 * m / 4];
    int k = 0;
    for(int i = 0; i < m; i++)
    {
        if(measures[i] > q1 && measures[i] < q3)
        {
            measures[k] = measures[i];
            k++;
        }
    }
    
    printf("droped %d items \n", m-k);
    m = k;

    // calculate mean value
    float mean = 0;
    for(int i = 0; i < m; i++)
        mean += measures[i];
    mean /= m;

    // calculate median value
    float median = 0;
    if(m % 2 == 0)
        median = (measures[m / 2] + measures[m / 2 - 1]) / 2;
    else
        median = measures[m / 2];

    // calculate unbiased sample variance
    float var = 0;
    for(int i = 0; i < m; i++)
        var += (measures[i] - mean) * (measures[i] - mean);
    var /= (m - 1);
    
    // calculate standard deviation
    float std_dev = sqrt(var);

    // calculate standard error
    float std_err = std_dev / sqrt(m);

    // calculate relative standart error
    float rel_std_err = (std_err / mean) * 100;

    printf("mean: %f \nmedian: %f \nstd_dev: %f \nstd_err: %f \nrel_std_err: %f\n", mean, median, std_dev, std_err, rel_std_err);

    printf("t = mean +- 3*var = %f +- %f\n", mean, 3*var);
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

    int m = 100;

    // warmup and correct check    
    test_func();
    check_correct();

    clock_t begin_cpu, end_cpu;
    struct timeval begin_time, end_time;

    // m-iterations
    float measures_real[m];
    float measures_cpu[m];

    for(int i = 0; i < m; i++)
    {
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

        measures_cpu[i] = cputime_spent;
        measures_real[i] = time_elapsed;
    }
    
    puts("CPU time:");
    print_statistics((float *)measures_cpu, m);
    puts("Real time:");
    print_statistics((float *)measures_real, m);
    
    return 0;
}

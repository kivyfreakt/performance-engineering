// naive floyd warshall implementation
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
// #include <immintrin.h> // avx
// #include <omp.h>
#include <time.h>

#define n 1024
#define B 32

#define INF 999999999

int **matrix;

void test_func()
{
    int i, j, k;
    for (k = 0; k < n; k++)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (matrix[i][k] + matrix[k][j] < matrix[i][j])
                {
                    matrix[i][j] = matrix[i][k] + matrix[k][j];
                }
            }
        }
    }
}

void graph_init_random()
{
     unsigned int i, j;

     srand48(5051);
     for(i=0; i<n; i++)
        for(j=0; j<n; j++)
        {   
            if(i == j)
                matrix[i][j] = INF;
            else
                matrix[i][j] = abs(((int)lrand48()) % 1048576);
        }
}


int main()
{
    
    int i;
    matrix = (int **)malloc(n*sizeof(int *));
    for(i = 0; i < n; i++)
        matrix[i] = (int *)malloc(n*sizeof(int));

    graph_init_random();
    // warmup
    test_func();
    graph_init_random();
    test_func();
    
    for(i = 0; i < n; i++)
        free(matrix[i]);

    free(matrix);

    return 0;
}
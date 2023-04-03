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

void FW(int **A, int K, int I, int J, int N)
{
     int i,j,k;

     for(k=K; k<K+N; k++)
        for(i=I; i<I+N; i++)
           for(j=J; j<J+N; j++)
                if (A[i][k] + A[k][j] < A[i][j])
                    A[i][j] = A[i][k] + A[k][j];

}

void test_func()
{
    int i, j, k;

    for(k=0;k<n;k+=B)
    {
        FW(matrix,k,k,k,B);

        #pragma omp parallel for
        for(i=0; i<k; i+=B)
            FW(matrix,k,i,k,B);
        #pragma omp parallel for
        for(i=k+B; i<n; i+=B)
            FW(matrix,k,i,k,B);
        #pragma omp parallel for
        for(j=0; j<k; j+=B)
            FW(matrix,k,k,j,B);
        #pragma omp parallel for
        for(j=k+B; j<n; j+=B)
            FW(matrix,k,k,j,B);
        #pragma omp parallel for private(j)
        for(i=0; i<k; i+=B)
            for(j=0; j<k; j+=B)
                FW(matrix,k,i,j,B);
        #pragma omp parallel for private(j)
        for(i=0; i<k; i+=B)
            for(j=k+B; j<n; j+=B)
                FW(matrix,k,i,j,B);
            #pragma omp parallel for private(j)
        for(i=k+B; i<n; i+=B)
            for(j=0; j<k; j+=B)
                FW(matrix,k,i,j,B);
        #pragma omp parallel for private(j)
        for(i=k+B; i<n; i+=B)
            for(j=k+B; j<n; j+=B)
                FW(matrix,k,i,j,B);
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
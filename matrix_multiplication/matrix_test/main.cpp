#include <iostream>
#include "omp.h"

int a[50][80];
int b[80][50];
int c0[50][50];
int c1[50][50];
int c2[50][50];

void gen_matrix()
{
    int i,j;
    for(i=0;i<50;i++)
    {
        for(j=0;j<80;j++)
        {
            a[i][j] = rand()%100;
        }
    }
    for(i=0;i<80;i++)
    {
        for(j=0;j<50;j++)
        {
            b[i][j] = rand()%100;
        }
    }
}

void matrix_multi_serial()
{
    int i,j,k;
    double start = omp_get_wtime();
    for(i=0;i<50;i++)
    {
        for(j=0;j<50;j++)
        {
            for(k=0;k<80;k++)
            {
                c0[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    printf("Serial : %lf us\n", (end-start)*1000000);
}

void matrix_multi_parallel1()
{
    //Static Scheduler
    memset(c1,0,sizeof c1);
    int i,j,k;
    double start = omp_get_wtime();
    #pragma omp parallel for schedule(static,50) collapse(2) private(i,j,k)shared(a,b,c1)
    for(i=0;i<50;i++)
    {
        for( j=0;j<50;j++)
        {
            for(k=0;k<80;k++)
            {
                c1[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    printf("Parallel(Static Scheduler) %lf us\n", (end-start)*1000000);
}

void matrix_multi_parallel2()
{
    //Dynamic Scheduler
    memset(c2,0,sizeof c2);
    int i,j,k;
    double start = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic,10) collapse(2) private(i,j,k)shared(a,b,c2)
    for(i=0;i<50;i++)
    {
        for( j=0;j<50;j++)
        {
            for(k=0;k<80;k++)
            {
                c2[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    printf("Parallel(Dynamic Scheduler) %lf us\n", (end-start)*1000000);
}

int main()
{
    gen_matrix();
    matrix_multi_serial();
    matrix_multi_parallel1();
    matrix_multi_parallel2();
    return 0;
}

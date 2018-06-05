#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define SIZE 2048

double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }

    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main (int argc, char **argv) {

    srand(time(NULL));
    static int a[SIZE][SIZE]={0}, b[SIZE][SIZE]={0}, c[SIZE][SIZE]={0};
    int i, j, k;
    struct timespec start, end;

    /* create matrix */
    for (i=0;i<SIZE;i++) {
        for (j=0;j<SIZE;j++)
            a[i][j]=rand()%1000;
    }

    for (i=0;i<SIZE;i++) {
        for (j=0;j<SIZE;j++) 
            b[i][j]=rand()%1000;
    }

    /* matrix multiply */
    clock_gettime(CLOCK_REALTIME, &start);
    #pragma omp parallel for collapse(3)
    for (i=0;i<SIZE;i++) {
        for (j=0;j<SIZE;j++) 
            for (k=0;k<SIZE;k++)
                c[i][j]+=a[i][k]*b[k][j];
    }
    clock_gettime(CLOCK_REALTIME, &end);
    printf("parallel : %f sec\n",diff_in_second(start, end));

}

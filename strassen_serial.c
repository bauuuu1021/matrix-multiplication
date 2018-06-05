#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void add (int a[][SIZE/2], int b[][SIZE/2], int c[][SIZE/2]) {
    int i,j;
    for (i=0;i<SIZE/2;i++)
        for (j=0;j<SIZE/2;j++)
            c[i][j]=a[i][j]+b[i][j];
}

void sub (int a[][SIZE/2], int b[][SIZE/2], int c[][SIZE/2]) {
    int i,j;
    for (i=0;i<SIZE/2;i++)
        for (j=0;j<SIZE/2;j++)
            c[i][j]=a[i][j]-b[i][j];
}

void mul (int a[][SIZE/2], int b[][SIZE/2], int c[][SIZE/2]) {
    int i,j,k;
    for (i=0;i<SIZE/2;i++)
        for (j=0;j<SIZE/2;j++)
            for (k=0;k<SIZE/2;k++)
                c[i][j]+=a[i][k]*b[k][j];
}

void strassen (int a[][SIZE], int b[][SIZE], int c[][SIZE]) {
    static int m1[SIZE/2][SIZE/2]={0}, m2[SIZE/2][SIZE/2]={0}, m3[SIZE/2][SIZE/2]={0}, \
    m4[SIZE/2][SIZE/2]={0}, m5[SIZE/2][SIZE/2]={0}, m6[SIZE/2][SIZE/2]={0}, m7[SIZE/2][SIZE/2]={0};
    static int a11[SIZE/2][SIZE/2]={0}, a12[SIZE/2][SIZE/2]={0}, a21[SIZE/2][SIZE/2]={0}, a22[SIZE/2][SIZE/2]={0};
    static int b11[SIZE/2][SIZE/2]={0}, b12[SIZE/2][SIZE/2]={0}, b21[SIZE/2][SIZE/2]={0}, b22[SIZE/2][SIZE/2]={0};
    static int c11[SIZE/2][SIZE/2]={0}, c12[SIZE/2][SIZE/2]={0}, c21[SIZE/2][SIZE/2]={0}, c22[SIZE/2][SIZE/2]={0};
    static int temp1[SIZE/2][SIZE/2]={0}, temp2[SIZE/2][SIZE/2]={0};
    int i, j;

    /* seperate matrix */
    for (i=0;i<SIZE/2;i++) {
        for (j=0;j<SIZE/2;j++) {
            a11[i][j]=a[i][j];
            b11[i][j]=b[i][j];
            a12[i][j]=a[i][j+SIZE/2];
            b12[i][j]=b[i][j+SIZE/2];
            a21[i][j]=a[i+SIZE/2][j];
            b21[i][j]=b[i+SIZE/2][j];
            a22[i][j]=a[i+SIZE/2][j+SIZE/2];
            b22[i][j]=b[i+SIZE/2][j+SIZE/2];
        }
    }
    
    /* M1 = (A11 + A22) (B11 + B22) */
    add(a11,a22,temp1);
    add(b11,b22,temp2);
    mul(temp1,temp2,m1);
    /* M2 = (A21 + A22)B11 */
    add(a21,a22,temp1);
    mul(temp1,b11,m2);
    /* M3 = A11(B12 − B22) */
    sub(b12,b22,temp1);
    mul(a11,temp1,m3);
    /* M4 = A22(B21 − B11) */
    sub(b21,b11,temp1);
    mul(a22,temp1,m4);
    /* M5 = (A11 + A12)B22 */
    add(a11,a12,temp1);
    mul(temp1,b22,m5);
    /* M6 = (A21 − A11)(B11 + B12) */
    sub(a21,a11,temp1);
    add(b11,b12,temp2);
    mul(temp1,temp2,m6);
    /* M7 = (A12 − A22)(B21 + B22) */
    sub(a12,a22,temp1);
    add(b21,b22,temp2);
    mul(temp1,temp2,m7);

    /* C11 = M1 + M4 − M5 + M7 */
    add(m1,m4,temp1);
    sub(m7,m5,temp2);
    add(temp1,temp2,c11);
    /* C12 = M3 + M5 */
    add(m3,m5,c12);
    /* C21 = M2 + M4 */
    add(m2,m4,c21);
    /* C22 = M1 − M2 + M3 + M6 */
    sub(m1,m2,temp1);
    add(m3,m6,temp2);
    add(temp1,temp2,c22);

    /* combine matrix */
    for (i=0;i<SIZE/2;i++) {
        for (j=0;j<SIZE/2;j++) {
            c[i][j]=c11[i][j];
            c[i][j+SIZE/2]=c12[i][j];
            c[i+SIZE/2][j]=c21[i][j];
            c[i+SIZE/2][j+SIZE/2]=c22[i][j];
        }
    }
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
    strassen(a,b,c);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("strassen serial : %f sec\n",diff_in_second(start, end));

}

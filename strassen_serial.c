#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_SIZE 1024

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

void add (int size, int **a, int **b, int **c) {
    int i,j;
    for (i=0;i<size;i++)
        for (j=0;j<size;j++)
            c[i][j]=a[i][j]+b[i][j];
}

void sub (int size, int **a, int **b, int **c) {
    int i,j;
    for (i=0;i<size;i++)
        for (j=0;j<size;j++)
            c[i][j]=a[i][j]-b[i][j];
}

int** alloc_matrix(int size){
    int** matrix;
    matrix = (int**) malloc(size * sizeof(int *));

    for (int i = 0; i < size; i++)
    	matrix[i] = (int*) malloc(size * sizeof(int));
        
    return matrix;
}

void free_matrix(int **mat, int n) {

    int i;

    if (mat == NULL) 
        return;

    for (i = 0; i < n; i++) 
        free(mat[i]); 

    free(mat);         
    mat = NULL;
}

void strassen (int size, int **a, int **b, int **c) {

    int **m1, **m2, **m3, **m4, **m5, **m6, **m7, **a11, **a12, **a21, **a22,
        **b11, **b12, **b21, **b22, **c11, **c12, **c21, **c22, **temp1, **temp2;
    int i, j, k;
    int newSize;

    /* size==2, multiply it directly */
    if (size==2) {
        /* clear c[i][j] */
        for (i=0;i<size;i++)
            for (j=0;j<size;j++)
                c[i][j]=0;

        for (i=0;i<size;i++)
            for (j=0;j<size;j++)
                for (k=0;k<size;k++)
                    c[i][j]+=a[i][k]*b[k][j];

        return;
    }

    /* alloc memory */
    newSize = size/2;
    temp1 = alloc_matrix(newSize);
    temp2 = alloc_matrix(newSize);
    m1 = alloc_matrix(newSize);
    m2 = alloc_matrix(newSize);
    m3 = alloc_matrix(newSize);
    m4 = alloc_matrix(newSize);
    m5 = alloc_matrix(newSize);
    m6 = alloc_matrix(newSize);
    m7 = alloc_matrix(newSize);
    a11 = alloc_matrix(newSize);
    a12 = alloc_matrix(newSize);
    a21 = alloc_matrix(newSize);
    a22 = alloc_matrix(newSize);
    b11 = alloc_matrix(newSize);
    b12 = alloc_matrix(newSize);
    b21 = alloc_matrix(newSize);
    b22 = alloc_matrix(newSize);
    c11 = alloc_matrix(newSize);
    c12 = alloc_matrix(newSize);
    c21 = alloc_matrix(newSize);
    c22 = alloc_matrix(newSize);

    /* seperate matrix */
    for (i=0;i<newSize;i++) {
        for (j=0;j<newSize;j++) {
            a11[i][j]=a[i][j];
            b11[i][j]=b[i][j];
            a12[i][j]=a[i][j+newSize];
            b12[i][j]=b[i][j+newSize];
            a21[i][j]=a[i+newSize][j];
            b21[i][j]=b[i+newSize][j];
            a22[i][j]=a[i+newSize][j+newSize];
            b22[i][j]=b[i+newSize][j+newSize];
        }
    }
    
    /* M1 = (A11 + A22) (B11 + B22) */
    add(newSize,a11,a22,temp1);
    add(newSize,b11,b22,temp2);
    strassen(newSize,temp1,temp2,m1);
    /* M2 = (A21 + A22)B11 */
    add(newSize,a21,a22,temp1);
    strassen(newSize,temp1,b11,m2);
    /* M3 = A11(B12 − B22) */
    sub(newSize,b12,b22,temp1);
    strassen(newSize,a11,temp1,m3);
    /* M4 = A22(B21 − B11) */
    sub(newSize,b21,b11,temp1);
    strassen(newSize,a22,temp1,m4);
    /* M5 = (A11 + A12)B22 */
    add(newSize,a11,a12,temp1);
    strassen(newSize,temp1,b22,m5);
    /* M6 = (A21 − A11)(B11 + B12) */
    sub(newSize,a21,a11,temp1);
    add(newSize,b11,b12,temp2);
    strassen(newSize,temp1,temp2,m6);
    /* M7 = (A12 − A22)(B21 + B22) */
    sub(newSize,a12,a22,temp1);
    add(newSize,b21,b22,temp2);
    strassen(newSize,temp1,temp2,m7);

    /* C11 = M1 + M4 − M5 + M7 */
    add(newSize,m1,m4,temp1);
    sub(newSize,m7,m5,temp2);
    add(newSize,temp1,temp2,c11);
    /* C12 = M3 + M5 */
    add(newSize,m3,m5,c12);
    /* C21 = M2 + M4 */
    add(newSize,m2,m4,c21);
    /* C22 = M1 − M2 + M3 + M6 */
    sub(newSize,m1,m2,temp1);
    add(newSize,m3,m6,temp2);
    add(newSize,temp1,temp2,c22);

    /* combine matrix */
    for (i=0;i<newSize;i++) {
        for (j=0;j<newSize;j++) {
            c[i][j]=c11[i][j];
            c[i][j+newSize]=c12[i][j];
            c[i+newSize][j]=c21[i][j];
            c[i+newSize][j+newSize]=c22[i][j];
        }
    }

    /* free memory */
    free_matrix(temp1,newSize);
    free_matrix(temp2,newSize);
    free_matrix(m1,newSize);
    free_matrix(m2,newSize);
    free_matrix(m3,newSize);
    free_matrix(m4,newSize);
    free_matrix(m5,newSize);
    free_matrix(m6,newSize);
    free_matrix(m7,newSize);
    free_matrix(a11,newSize);
    free_matrix(a12,newSize);
    free_matrix(a21,newSize);
    free_matrix(a22,newSize);
    free_matrix(b11,newSize);
    free_matrix(b12,newSize);
    free_matrix(b21,newSize);
    free_matrix(b22,newSize);
    free_matrix(c11,newSize);
    free_matrix(c12,newSize);
    free_matrix(c21,newSize);
    free_matrix(c22,newSize);

}

int main (int argc, char **argv) {

    srand(time(NULL));
    int **a, **b, **c;
    int i, j, k;
    struct timespec start, end;

    /* alloc memory */
    a = alloc_matrix(INPUT_SIZE);
    b = alloc_matrix(INPUT_SIZE);
    c = alloc_matrix(INPUT_SIZE);

    /* create matrix */
    for (i=0;i<INPUT_SIZE;i++) {
        for (j=0;j<INPUT_SIZE;j++) 
            a[i][j]=rand()%100;
    }

    for (i=0;i<INPUT_SIZE;i++) {
        for (j=0;j<INPUT_SIZE;j++)  
            b[i][j]=rand()%100;
    }

    /* matrix multiply */
    clock_gettime(CLOCK_REALTIME, &start);
    strassen(INPUT_SIZE,a,b,c);
    clock_gettime(CLOCK_REALTIME, &end);
    printf("strassen serial : %f sec\n",diff_in_second(start, end));

}

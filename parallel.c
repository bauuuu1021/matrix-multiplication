#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <omp.h>

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

    int num;
    FILE *fp = NULL;
    int i, j, k;
    int a_row, a_col, b_row, b_col;
    int **a, **b, **c;
    char a_buff_row[255], a_buff_col[255], b_buff_row[255], b_buff_col[255], buff[255];
    struct timespec start, end;
    memset(a_buff_row,0,255);
    memset(a_buff_col,0,255);
    memset(b_buff_row,0,255);
    memset(b_buff_col,0,255);
    memset(buff,0,255);
    
    /* input file : default or cmd arguments */
    fp = (argc==1)? fopen("input.txt", "r"):fopen(argv[1], "r");

    /* scan matrix a */
    fscanf(fp,"%s",a_buff_row);
    a_row = atoi(a_buff_row);
    fscanf(fp,"%s",a_buff_col);
    a_col = atoi(a_buff_col);
    
    a = (int**)malloc(sizeof(int*) * a_row);
    for (i=0;i<a_row;i++)
        a[i] = (int*)malloc(sizeof(int) * a_col);
    
    for(i = 0;i<a_row;i++){
        for(j = 0;j<a_col;j++){
            fscanf(fp,"%s",buff);
            num = atoi(buff);
            a[i][j] = num;
            memset(buff,0,255);
        }
    }

    /* scanf matrix b */
    fscanf(fp,"%s",b_buff_row);
    b_row = atoi(b_buff_row);
    fscanf(fp,"%s",b_buff_col);
    b_col = atoi(b_buff_col);
    
    b = (int**)malloc(sizeof(int*) * b_row);
    for (i=0;i<b_row;i++)
        b[i] = (int*)malloc(sizeof(int) * b_col);
    
    for(i = 0;i<b_row;i++){
        for(j = 0;j<b_col;j++){
            fscanf(fp,"%s",buff);
            num = atoi(buff);
            b[i][j] = num;
            memset(buff,0,255);
        }
    }
    fclose(fp);

    /* create matrix c to store result */
    c = (int**)malloc(sizeof(int*) * a_row);
    for (i=0;i<a_row;i++)
        c[i] = (int*)malloc(sizeof(int) * b_col);

    for(i = 0;i<a_row;i++){
        for(j = 0;j<b_col;j++){
            c[i][j]=0;
        }
    }

    /* matrix multiply */ 
    clock_gettime(CLOCK_REALTIME, &start);
    
    #pragma omp parallel for collapse(3) 
    for (i=0;i<a_row;i++) 
        for (j=0;j<b_col;j++) 
            for (k=0;k<a_col;k++) 
                c[i][j]+=a[i][k]*b[k][j];

    clock_gettime(CLOCK_REALTIME, &end);
    printf("parallel\t  : %f sec\n",diff_in_second(start, end));

    /* export file : parallel.txt */
    FILE *fp_out = NULL;
    fp_out = fopen("parallel.txt", "w");
    for(i = 0;i<a_row;i++){
        for(j = 0;j<b_col;j++){
            fprintf(fp_out,"%d ",c[i][j]);
        }
        fprintf(fp_out,"\n");
    }
    fclose(fp_out);

    /* free memory */
    for (i=0;i<a_row;i++)
        free(a[i]);
    for (i=0;i<b_row;i++)
        free(b[i]);
    free(a);
    free(b);    

}
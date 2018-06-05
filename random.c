#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void outputMatrix (FILE *fp, int size) {
    int i,j;

    fprintf(fp,"%d %d\n",size,size);
        for (i=0;i<size;i++) {
            for (j=0;j<size;j++){
                fprintf(fp,"%d ",rand()%1000);
            }
            fprintf(fp,"\n");
        }
}

int main (int argc, char **argv) {

    if (argc!=1 && argc!=3) {
        printf("Incorrect format!\nPlease input SIZE and OUTPUTFILE as argv\n");
        return 1;
    }

    srand(time(NULL));
    FILE * fp = (argc==1)? fopen("matrix.txt","w"):fopen(argv[2],"w");    /* default path input.txt */
    int size = (argc==1)? 10:atoi(argv[1]);  /* default size is 10 */

    outputMatrix(fp, size);
    outputMatrix(fp, size);

    fclose(fp);
    return 0;
}

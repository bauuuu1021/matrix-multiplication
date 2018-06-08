#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void outputMatrix (FILE *fp, int size) {
    int i,j;

    fprintf(fp,"%d %d\n",size,size);
        for (i=0;i<size;i++) {
            for (j=0;j<size;j++){
                fprintf(fp,"%d ", rand()%100);
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
    int size = (argc==1)? 16:atoi(argv[1]);  /* default size is 16 */
    FILE * fp = (argc==1)? fopen("input.txt","w"):fopen(argv[2],"w");    /* default path input.txt */
    
    outputMatrix(fp, size);
    outputMatrix(fp, size);

    fclose(fp);
    return 0;
}

#include "dependencies.h"                               // dependencies

int main(int argc, char *argv[]){                       // main function starts here
    FILE *fp;                                           // file pointer
    char *line;                                         // pointer to a line 
    long unsigned int l = 0;                            // length of a line                       
    for(int i=1;i<argc-1;i++){                          // going through all the files
        int j=0;                                        // rechecking if there is any trailing \n in the arguments
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
        argv[i][j] = '\0';                              // each argument ends in \0
        fp = fopen(argv[i],"r");                        // open file in read mode                   
        // opening the file in the location. if the file is in the pwd, then it will be opened even if the complete path is not mentioned
        // if the file is not in the pwd, then the complete path is given by the user
        if(fp==NULL){                                   // if the file does not exist
            printf("\033[1;31mcat : file not found at : %s\n\033[0m",argv[i]);
            continue;
        }
        printf("\033[1;36mPrinting File: %s\033[0m\n",argv[i]);
        while(getline(&line,&l,fp)>=0)printf("%s",line); // print the complete file
        printf("\n\n");
    }
    return EXIT_SUCCESS;
}
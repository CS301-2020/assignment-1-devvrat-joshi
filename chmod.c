#include "dependencies.h"                   // dependencies

int main(int argc, char *argv[]){           // main function starts here
    if(strlen(argv[1])!=3){                 // if the argument for chmod is different length then 3
        printf("\033[1;31mchmod : Wrong Mode : %s\n",argv[1]);  
        return EXIT_SUCCESS;                
    }
    int j=0;                                // ensuring that there is no trailing \n
    while(argv[2][j]!='\0'&&argv[2][j]!='\n'&&argv[2][j]!=' ')j++;
    argv[2][j] = '\0';                      // the file name or path ends in \0
    if(chmod(argv[2],strtol(argv[1],NULL,8))!=0)                 // if chmod is success
        printf("chmod : failed changing mode of %s\n",argv[2]);     
    else                                                         // if chmod fails 
        printf("\033[1;32mchmod success\n");
    return EXIT_SUCCESS;                                         // exit success
}
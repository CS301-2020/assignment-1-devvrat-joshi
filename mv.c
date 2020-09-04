#include "dependencies.h"               // dependencies

int main(int argc, char *argv[]){       // main function starts here
    if(argc<4){
        printf("\033[1;33mmv [source file|directory] [destination directory]\n");
    }
    FILE *fp;                           // file pointers
    int j=0;                            // ensuring that the destination path has no trailing \n
    while(argv[argc-2][j]!='\0'&&argv[argc-2][j]!='\n'&&argv[argc-2][j]!=' ')j++;  
    argv[argc-2][j] = '\0';             // destination path given ends in \0 
    for(int i=1;i<argc-2;i++){          // looping through each mentioned file
        char dest_path[1000];           // path of the destination
        int j=0;                        // ensuring that the arugments has no trailing \n
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
        argv[i][j] = '\0';              // arguments given ends in \0 
        char filename[100];             // filename of the file that is to be moved
        int this = 0;                   // if the argument is a path, then finding
        for(j=0;j<strlen(argv[i]);j++){ // looping through argument until we get the last '/'
            if(argv[i][j]=='/')this=j;
        }
        if(this!=0){                    // if the argument contains '/'
            j = this+1;                 // from the next character after last '/'
            while(argv[i][j]!='\0'){    // looping until we reach the end of argument
                filename[j-this-1] = argv[i][j];
                j++;
            }
            // by this way we extract the filename from the path
            filename[j-this-1] = '\0';  
            strcpy(dest_path,argv[argc-2]);   // making the destination path
            strcat(dest_path,"/");            // making the destination path
            strcat(dest_path,filename);       // making the destination path
        }
        if(access(argv[i],F_OK)==-1){         // if such a path does not exist
            strcpy(dest_path,argv[argc-2]);   // then the given argument was a continuation of the pwd
            strcat(dest_path,"/");            // making the destination path
            strcat(dest_path,argv[i]);        // making the destination path
        }
        printf("\033[1;33m%s\n",dest_path);             // print the destination path
        int x = rename(argv[i],dest_path);              // rename the path from source path to destination path
        if(x!=0)printf("\033[1;31mmv : File not moved : %s\n\033[0m",argv[i]);
        else printf("\033[1;36mmoved : %s\n",argv[i]);
    }
    return EXIT_SUCCESS;
}

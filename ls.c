#include "dependencies.h"                   // include dependencies

int main(int argc, char *argv[]){           // main function starts here
    struct dirent *dnt;                     // struct dirent
    if(argc==2){                            // if ls has no argument, please note an extra argument is passed by shell as the path of the current process   
        DIR *dr= opendir(".");              // open current directory
        while((dnt = readdir(dr))!=NULL){   // readdir through all the internal files
            if(strcmp(dnt->d_name,".")!=0&&strcmp(dnt->d_name,"..")!=0){
                struct stat st;             // struct stat
                stat(dnt->d_name,&st);      // stat this path
                if(S_ISREG(st.st_mode)==0)printf("\033[1;34m%s\n",dnt->d_name); // if it is directory print in other color
                else printf("\033[1;32m%s\n",dnt->d_name);
            }
            
        }
    }
    else{
        for(int i=1;i<argc-1;i++){          // ls has arugments, so go through all the arguments
            int j=0;                        // ensuring that the arugment has no trailing \n
            while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
            argv[i][j] = '\0';              
            struct stat st;                 // struct stat
            int x = stat(argv[i],&st);      // stat the path
            if(x==-1){                      // if it does not exist then error message
                printf("\033[1;31mls : No such directory or file : '%s'\n\033[0m",argv[i]);
                continue;
            }
            if(S_ISREG(st.st_mode)!=0){     // if it is a file, then print the name of file
                printf("\033[1;32m%s\n",argv[i]);
                continue;
            }
            printf("\033[1;36mcontents of : %s\n",argv[i]);     // if it is a folder, then print contents of folder
            DIR *dr= opendir(argv[i]);                          // opendir the folder
            while((dnt = readdir(dr))!=NULL){                   // readdir through all the contents of folder
                if(strcmp(dnt->d_name,".")!=0&&strcmp(dnt->d_name,"..")!=0){
                    char ispath_dir[1000];                      // make a path to the file
                    strcpy(ispath_dir,argv[i]);                 // make a path to the file
                    strcat(ispath_dir,"/");                     // make a path to the file
                    strcat(ispath_dir,dnt->d_name);             // make a path to the file
                    stat(ispath_dir,&st);                       // stat the path
                    if(S_ISREG(st.st_mode)==0)printf("\033[1;34m%s\n",dnt->d_name);     // if it is directory print in other color
                    else printf("\033[1;32m%s\n",dnt->d_name);
                }
            }   
            printf("\n");
        }
    }
    return 0;
}
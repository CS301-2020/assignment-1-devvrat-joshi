#include "dependencies.h"           //dependencies

long long int total = 0;            // total number of files
long long int directories = 0;      // total number of directories

void recurse(char *path){           // takes path and deletes everything in that path
    struct dirent *dnt;             // direct struct to access the directory
    DIR *dr= opendir(path);         // opening directory
    while((dnt = readdir(dr))!=NULL){                                   // readdir used to loop through the files
        if(strcmp(dnt->d_name,".")!=0&&strcmp(dnt->d_name,"..")!=0){    // if the directory is not '.' and '..'
            struct stat st;                         // stat struct
            char folder_path[1000];                 // path of the file
            strcpy(folder_path,path);               // constructing the path
            strcat(folder_path,"/");                // constructing the path
            strcat(folder_path,dnt->d_name);        // constructing the path
            stat(folder_path,&st);                  // stat the path
            if(S_ISREG(st.st_mode)==0){             // if the path is a directory
                char path_append[1000];             // path to the internal directory
                strcpy(path_append,path);           // constructing the path
                strcat(path_append,"/");            // constructing the path
                strcat(path_append,dnt->d_name);    // constructing the path
                recurse(path_append);               // recurse on this subdirectory
                remove(path_append);                // remove this directory
                directories++;                      // increment the number of deleted directory
            }
            else{                                   // else it is a file
                int x = remove(folder_path);        // remove the file
                if(x==0)total++;                    // increment the number of files directory
                else printf("\033[1;31mmkdir: Failed Deleting Directory : %s\n\033[0m",folder_path);
            }
        }
    }
}

int main(int argc, char* argv[]){                                   // main function starts here
    if(argc<=2){                            // argv[0] = rm and argv[1] = (path):shell.c passed this argument
        printf("\033[1;31mrm [option] [filepath|dirpath]\033[0m\n");
        exit(0);
    }
    int j=0;                                                        // ensure that the argument has no trailing \n
    while(argv[1][j]!='\0'&&argv[1][j]!='\n'&&argv[1][j]!=' ')j++;
    argv[1][j] = '\0';
    if(strcmp(argv[1],"-r")==0){                                    // if first argument is -r
        if(argv[3]==NULL){                                          // check if the argv[3] is null
            printf("\033[1;33mrm -r [directory]\n");                // if so then give the format for rm
            exit(0);                                                // exit
        }
        for(int i=2;i<argc-1;i++){                                  // looping through all arguments
            printf("\033[1;33mPermanently delete everything in the directory : %s\nY/N : ",argv[i]);
            char *confirm = NULL;               // confirm if you want to delete entire directory
            long unsigned int l = 0;            // length of input
            getline(&confirm,&l,stdin);         // get confirmation
            if(strcmp(confirm,"Y\n")==0)        // if yes, then recurse on argv[2]
                recurse(argv[i]);               
            else{                               // else abort operation
                printf("\033[1;31mAbort\n");
                return EXIT_SUCCESS;
            }
            remove(argv[i]);                    // remove the directory at the end
            printf("\033[1;33mTotal files deleted : %lld\n",total); 
            printf("\033[1;33mTotal directories deleted : %lld\n",directories+1);            
        }
        exit(0);
    }
    // if -r is not the option
    for(int i=1;i<argc-1;i++){          // loop through all the files  
        int j=0;                        // ensure that there is no trailing \n at the end of argument
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
        argv[i][j] = '\0';
        int x = remove(argv[i]);        // remove file
        if(x==0)printf("\033[1;36mFile Deleted at: %s\n\033[0m",argv[i]);
        else printf("\033[1;31mrm: Failed Deleting Directory : %s\n\033[0m",argv[i]);
    }
    return EXIT_SUCCESS;
}
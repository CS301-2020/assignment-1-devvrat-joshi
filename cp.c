#include "dependencies.h"                   // dependencies

void dir_copy(int, char**);                 // if -r option

int main(int argc, char *argv[]){
    if(argc<4){     // arg1:cp, arg2:source, arg3: destination, arg4:(given by shell.c) so there should be atleast 4 arguments
        printf("\033[1;31mcp [option] [source] [destination]\033[0m\n");    
    }
    FILE *fp, *dp;                  // file pointers
    int j=0;                        // ensuring that there is no trailing 0 in the argument
    while(argv[argc-2][j]!='\0'&&argv[argc-2][j]!='\n'&&argv[argc-2][j]!=' ')j++;
    argv[argc-2][j] = '\0';         
    if(strcmp(argv[1],"-r")==0){    // if the option is -r
        dir_copy(argc,argv);        // then copy the dirs in source to dirs in destination
        return EXIT_SUCCESS;
    }
    // if -r option is not there
    char *line = NULL;              // pointer to the line in source
    long unsigned int l = 0;        // length of line
    for(int i=1;i<argc-2;i++){      // loop through the entire argument source list
        char folder_path[1000];     // source path
        char dest_path[1000];       // destination path
        int j=0;                    // ensuring there is no trailing \n
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
        argv[i][j] = '\0';
        char filename[100];         // getting the name of the file
        int this = 0;               // getting the last / position in argument
        for(j=0;j<strlen(argv[i]);j++){
            if(argv[i][j]=='/')this=j;
        }
        if(this!=0){                // if / is there in arugment
            j = this+1;             // get the name of the file i.e. after the last /
            while(argv[i][j]!='\0'){
                filename[j-this-1] = argv[i][j];
                j++;
            }
            filename[j-this-1] = '\0';
            strcpy(dest_path,argv[argc-2]);     // make the destination path as dest/filename
            strcat(dest_path,"/");              // make the destination path as dest/filename
            strcat(dest_path,filename);         // make the destination path as dest/filename
        }
        else{ // if there is no / in path
            strcpy(dest_path,argv[argc-2]);     // make the destination path as dest/arg
            strcat(dest_path,"/");              // make the destination path as dest/arg
            strcat(dest_path,argv[i]);          // make the destination path as dest/arg
        }
        struct stat st;                         // struct stat
        stat(argv[i],&st);                      // stat arg
        if(S_ISREG(st.st_mode)==0){             // if arg is directory then give argument -r
            printf("\033[1;31mFor directory use -r command, %s is a directory\033[0m\n",argv[i]);
            continue;
        }
        printf("%s\n",dest_path);               // print the path where file is copied
        fp = fopen(argv[i],"r");                // open source
        dp = fopen(dest_path,"w");              // open destination
        if(fp==NULL){                           // if source does not exist
            printf("\033[1;31mcp : file not found at : %s\n\033[0m",argv[i]);
            continue;
        }
        if(dp==NULL){                           // if problem in creating destination
            printf("\033[1;31mcp : file not found at : %s\n\033[0m",dest_path);
            continue;
        }
        while(getline(&line,&l,fp)>=0)fprintf(dp,"%s",line);    // copy file
        printf("\033[1;36mCopied Successfully: %s\033[0m\n",argv[i]);       
    }
    return EXIT_SUCCESS;
}

void dir_copy(int argc, char **argv){
    struct dirent *dnt;     // struct dirent
    FILE *fp, *dp;          // file pointers
    char *line = NULL;      // line
    long unsigned int l = 0;// length of line alloted by getline
    for(int i=2;i<argc-2;i++)   // looping through all input directories
    {   
        int j=0;                // ensuring there is no trailing \n
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;
        argv[i][j] = '\0';
        char filename[1000];     // filename
        char dest[1000];         // destination
        int this = 0;            // getting the name of file i.e. after last / in argument
        for(j=0;j<strlen(argv[i]);j++){ 
            if(argv[i][j]=='/')this=j;
        }
        if(this!=0){                // if / is there in arugment
            j = this+1;             // get the name of the file i.e. after the last /
            while(argv[i][j]!='\0'){
                filename[j-this-1] = argv[i][j];
                j++;
            }
            filename[j-this-1] = '\0';
            strcpy(dest,argv[argc-2]);     // make the destination path as dest/filename
            strcat(dest,"/");              // make the destination path as dest/filename
            strcat(dest,filename);         // make the destination path as dest/filename
        }
        else{ // if there is no / in path
            strcpy(dest,argv[argc-2]);     // make the destination path as dest/arg
            strcat(dest,"/");              // make the destination path as dest/arg
            strcat(dest,argv[i]);          // make the destination path as dest/arg
        }
        int x = mkdir(dest, S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);   // make folder destination in rwx for user rwx for group and rx for others
        if(x==0)printf("\033[1;36mDirectory Created at: %s\n\033[0m",argv[i ]);  // if folder created successfully
        else printf("\033[1;31mmkdir: Failed Creating Directory or it already exists : %s\n\033[0m",argv[i]); // if it fails
        DIR *dr= opendir(argv[i]);          // opendir the source directory
        while((dnt = readdir(dr))!=NULL){   // readdir each content of directory
            if(strcmp(dnt->d_name,".")!=0&&strcmp(dnt->d_name,"..")!=0){
                char ispath_dir[1000];              // path of source file
                strcpy(ispath_dir,argv[i]);         // path of source file
                strcat(ispath_dir,"/");             // path of source file
                strcat(ispath_dir,dnt->d_name);     // path of source file
                struct stat st;                     // struct stat
                stat(ispath_dir,&st);               // stat file
                if(S_ISREG(st.st_mode)==0)continue; // if stat is directory, then do nothing
                else {
                    char dest_path[1000];           // destination file path   
                    strcpy(dest_path,dest);         // destination file path
                    strcat(dest_path,"/");          // destination file path
                    strcat(dest_path,dnt->d_name);  // destination file path
                    fp = fopen(ispath_dir,"r");     // open source in read mode
                    dp = fopen(dest_path,"w");      // open destination in write mode               
                    if(fp==NULL){                   // if source not found
                        printf("\033[1;31mcp : file not found at : %s\n\033[0m",ispath_dir);
                        continue;
                    }
                    if(dp==NULL){                   // if destination not created
                        printf("\033[1;31mcp : file not found at : %s\n\033[0m",dest_path);
                        continue;
                    }
                    while(getline(&line,&l,fp)>=0)fprintf(dp,"%s",line);    // copy file
                    printf("\033[1;36mCopied Successfully: %s\033[0m\n",dnt->d_name);
                }
            }
        }   
        printf("\n");
    }
    
}
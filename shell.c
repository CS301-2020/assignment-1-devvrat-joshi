// please read the comments carefully, in order to minimize number of conditionals, there is very different method followed in making the shell
// This file is the main process .c file and contains functions for user input and argument parsing
#include "dependencies.h"         // including all the required header files

short int background = 0;         // is the process to be run in background, background = 1 means in background

int main(int argc, char *argv[]){ // main function starts here
    printf("\033c");              // clear screen when shell starts
    char *get_stdin = NULL;       // used as a pointer to the stdin input
    long unsigned int l = 0;      // stores the length alloted to the input by getline
    char command[MAX_LEN];        // Name of command
    getcwd(path,sizeof(path));    // get the current directory path
    char mybinaries_path[1000];   // the other binaries for this shell resides in this path
    strcpy(mybinaries_path,path); // current path is the address of the custon binaries and so mybinaries_path is made to be same as current path
    print_info();                 // terminal start print information of terminal
    while(1){                     
        get_stdin = NULL;              // everytime the input is taken, before it the pointer to input string is made NULL
        l = 0;                         // everytime the input is taken, before it the input length is made 0
        getcwd(path,sizeof(path));     // before input, everytime verify the current directory path
        ready_print();                 // terminal is ready to take input. Function is in dependencies.h
        getline(&get_stdin,&l,stdin);  // taking the input, where the stdin is stored and get_stdin made to point to the storage
        int i = 0;                     // loop variable
        while((get_stdin[i]!=' '&&get_stdin[i]!='\n') && i<l){command[i] = get_stdin[i];i++;} // finding the first word in input as it is our command
        command[i] = '\0';             // putting EOF at the end of command
        if(strcmp(command,"quit")==0){ // if command is quit
            print_exit();              // print exit info
            return EXIT_SUCCESS;       // exit the shell
        }
        // customls is the compiled binary of ls.c
        // customrm is the compiled binary of rm.c
        // customcp is the compiled binary of cp.c
        // custommv is the compiled binary of mv.c
        // customgrep is the compiled binary of grep.c
        // customcat is the compiled binary of cat.c
        // custommkdir is the compiled binary of mkdir.c
        // customchmod is the compiled binary of chmod.c
        // all the above binaries are made by the make file
        char check_if_custom_command[10000];               // if the command is custom command (not inbuilt), then there is a binary named custom{{command}} in the path : mybinaries_path
        strcpy(check_if_custom_command,mybinaries_path);   // so make a path as mybinaries_path/custom{{command}}
        strcat(check_if_custom_command,"/custom");         // so make a path as mybinaries_path/custom{{command}}
        strcat(check_if_custom_command,command);           // so make a path as mybinaries_path/custom{{command}}
        // searching for a binary file custom{{command}} makes less number of else if in the code. Also if you want to add new custom commads, then just make a new custom{{command_new}} binary and no need to change a single line in shell.c
        if(access(check_if_custom_command,F_OK)!=-1)execute_command(command,get_stdin,mybinaries_path, 0);   // if there exists such a binary file named custom{command}, then call the execute_custom_command
        else if(strcmp(command,"pwd")==0)printf("\033[1;32mcurrent directory:\033[0m %s\n",path);   // print current working directory or the current path of shell.c process
        else if(strcmp(command,"cd")==0)execute_cd(command, get_stdin);
        else if(strcmp(command,"clear")==0)system("clear"); // clear terminal, just for ease of reading, not asked to implement. used system command to do it.
        else if(strcmp(command,"devsh")==0)continue;        // my terminal name is devsh
        else if(strcmp(command,"")==0)continue;             // if pressing only enter, then do nothing
        else execute_command(command, get_stdin, mybinaries_path, 1);  // if none of the custom command matches to the user command, then go for inbuilt binaries
    }
    return EXIT_SUCCESS;        // main process exit
}

void execute_command(char *command, char *get_stdin, char *mybinaries_path, int is_inbuilt){ // execute command
    // command : the name of command
    // get_stdin : the stdin line that contains the user input which will be converted into arguments
    // mybinaries_path : path to the custom implemented command binaries
    // is_inbuilt : it is 0 for custom commands, else 1 for inbuilt binaries, if the command is not a custom implemented command, then 1 is passed as is_inbuilt variable
    int PID, error;                  // int varbles for child PID and error detection
    char *args[2000];                // will contain the arguments
    get_all_args(args,get_stdin);    // get all the arguments in words format from the given input string
    PID = fork();                    // create a child process
    if(PID<0){                       // if PID is less then 0, then child process not created
        fprintf(stderr, "Process cannot be created for command : %s\n",command);
    }
    else if(PID==0){                 // else it is a child process
        //child
        if(is_inbuilt==0){
            printf("\033[1;33mdevsh : running custom %s\033[0m\n",command);
            // the command is custom implemented command
            int last = 0;                             // variable to get the index of last argument
            while(args[last]!=NULL)last++; 
            args[last] = path;                        // make the argument next to the last argument as the path of the main process
            args[last+1] = NULL;                      // so now the list of arguments end at the next index than the previous end index
            char this_binary_path[1000];              // preparing path for the custom command binary
            strcpy(this_binary_path,mybinaries_path); // this_binary_path has mybinaries_path  
            strcat(this_binary_path,"/custom");       // appending /custom
            strcat(this_binary_path,command);         // this_binary_path = {{mybinaries_path}}/custom{{command}}
            error = execvp(this_binary_path,args);    // execute custom command binary with error containing the exit status
        }
        else if(is_inbuilt==1){            // check if the command is inbuilt binary, the 
            printf("\033[1;33mdevsh : running inbuilt binary %s\033[0m\n",command);
            error = execvp(command,args);  // execute is as inbuilt binary with error containing the exit status
        }
        if(error==-1)fprintf(stderr,"\033[1;31mCommand not found : process not created : %d\n\033[0m", errno); 
    }
    else{
        // parent, shell.c (main process)
        if(background==0)      // if background is not 1, then wait for child to complete execution
            wait(NULL);        // wait for child
        else{                  // background is true
            printf("\033[1;36mprocess running in background\033[0m\n"); 
            background = 0;    // not waiting for child to complete execution and background is again set to 0
        }
    }
}

void execute_cd(char *command, char *from_input){
    char *args[10];                               // getting arguments in args
    get_all_args(args,from_input);                // get all the arguments in words with this command
    int num_of_args = 0;                          // number of arguments
    while(args[num_of_args]!=NULL)num_of_args++;  // counting number of arguments
    if(num_of_args==2){                           // if number of arguments is 2
        strcat(path,"/");                         // making the path as if the argument path is continuation of the current path
        strcat(path,args[1]);                     // appending the args[1]
        int len = strlen(path);                   // getting the length of new path
        if(path[len-1]=='\n')path[len-1]='\0';    // the character next to the end of path is made as end of string
        if(access(path,F_OK)!=-1)chdir(path);     // if this path exists, then chdir to that path
        else if(access(args[1],F_OK)!=-1){        // if it does not exists, then it can happen that the argument path was not continuation of old path, but a completely new path from starting character, so check if it exists
            chdir(args[1]);                       // chdir to that path
            strcpy(path,args[1]);                 // make our current process path to args[1]
        }
        else printf("\033[1;31mcd : No such directory : '%s'\n\033[0m",args[1]);  // if both paths does not exists, then give error
    }
}

void get_all_args(char **args, char *from_input){
    int index = 0;                                      // the number of arguments
    char *command = (char *)malloc(sizeof(char)*100);   // each word of length atmost 100
    command = strtok(from_input," ");                   // strtok will return the first word from the string
    while(command!=NULL){                               // until we completely parse the input
        args[index++] = command;                        // each word is stored into the char pointer array args
        command = (char *)malloc(sizeof(char)*100);     // allocation for next word
        command = strtok(NULL," ");                     // strtok will return the string upto the next space
    }
    int j = 0;                                          // an int variable named j
    //it might happen that the last argument has a trailing '\n', so it is removed here
    while(args[index-1][j]!='\0'&&args[index-1][j]!='\n'&&args[index-1][j]!=' ')j++; 
    args[index-1][j] = '\0';                            // last argument ending in end of line
    if(strcmp(args[index-1],"&")==0){                // if last argument is &
        background = 1;                                 // then the process is background process          
        args[index-1] = NULL;                           // & is made NULL
    }
    else args[index] = NULL;                            // args ends in NULL
}
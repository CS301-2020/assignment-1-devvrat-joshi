#include<stdio.h>           
#include<sys/wait.h>        // wait
#include<sys/stat.h>        // stat
#include<dirent.h>          // readdir, opendir, closedir
#include<unistd.h>          // fork exec
#include<stdlib.h>          // malloc
#include<errno.h>           // errno
#include<time.h>            // time
#include<string.h>          // strlen, strcpy, strcat
#define MAX_LEN 15

char path[1000];            // global variable current directory path

void execute_command(char*, char*, char *, int);
void not_found(char *);
void get_all_args(char **, char *);
void execute_cd(char *, char *);

void print_info(){      // starting shell
    time_t t; 
    time(&t);
    printf("\nAssignment 1 Terminal devsh: %s", ctime(&t));
    printf("Author: Devvrat Joshi\n");
    printf("\033[1;36mTo exit, type quit and press enter.\033[0m\n");
}

void print_exit(){      // exiting shell
    time_t t; 
    time(&t);
    printf("\033[1;36mExiting Terminal devsh : %s\033[0m", ctime(&t));
}

void ready_print(){     // ready for input
    system("echo -n $(tput bold)$(tput setaf 5)CS301$(tput setaf 7)::$(tput setaf 4)$USER");
    printf("\033[1;37m:\033[1;32m%s\033[0m$ ",path);
}

void not_found(char *command){
    printf("%s: command not found. To get all the commands, type help and press enter.\n",command);
}
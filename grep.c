#include "dependencies.h"                               // dependencies

void findpattern(char *line, char *pattern, int size, int line_nos,int p);

int main(int argc, char *argv[]){                       // main function starts
    if(argc<=2){                            // argv[0] = grep and argv[1] = (path):shell.c passed this argument
        printf("\033[1;31mgrep [pattern] [filepath|(nothing)]\033[0m\n");
        exit(0);
    }
    FILE *fp;                                           // file pointer
    char *line;                                         // pointer to a line 
    long unsigned int l = 0;                            // length of a line
    char *pattern = argv[1];                            // pattern is taken from argument 1
    if(argc==3){                                        // if the number of argument is 3, then the argument 1 is grep second argument is pattern, thirs argument is passed by shell, that is the current path of process, so the argument of the file is not provided
        char *taking_from_terminal = NULL;              // getting in 
        long unsigned int l = 0;
        int len = getline(&taking_from_terminal,&l,stdin);
        findpattern(taking_from_terminal,pattern,len,1,strlen(pattern));
    }
    for(int i=2;i<argc-1;i++){                          // searching from argument 2 to argument argc-2
        long long int size = 1;                         // let the size of line = 1
        int j=0;                                        // integer j = 0
        while(argv[i][j]!='\0'&&argv[i][j]!='\n'&&argv[i][j]!=' ')j++;    // make the last arugment end in end of line
        argv[i][j] = '\0';                                                // make the last arugment end in end of line                                  // make the path to the files which is to be opened
        fp = fopen(argv[i],"r");                                          // open file in read mode
        // opening the file in the location. if the file is in the pwd, then it will be opened even if the complete path is not mentioned
        // if the file is not in the pwd, then the complete path is given by the user
        if(fp==NULL){                                                     // File does not exist
            printf("\033[1;31mgrep : file not found at : %s\n\033[0m",argv[i]);
            continue;
        }
        printf("\033[1;36mgrep File: %s, pattern: %s\033[0m\n",argv[i],argv[1]);  
        int p = strlen(pattern);                                            // string length of pattern
        int line_nos = 0;                                                   // Line number in file
        while(size>=0){                                                     // until size if >=0 continue reading the file
            size = getline(&line,&l,fp);                                    // get the line in line variable and size of line in size variable
            line_nos++;                                                     // increment the number of lines
            findpattern(line,pattern,size,line_nos,p);
        }
        printf("\n\n");                                                     // if next file starts, then give 2 line breaks
    }
    return EXIT_SUCCESS;
}
void findpattern(char *line, char *pattern, int size, int line_nos,int p){
    // line : line in which pattern is to be found
    // pattern : pattern that is given
    // size : size of line
    // line_nos : line number in file
    // p : length of pattern
    if(strstr(line,pattern)!=NULL && size>0){                       // if there is atleast one instance of patten in this line
        printf("\033[1;32mLine %d\033[1;34m :\033[0m ",line_nos);   // print line number
        for(int j = 0;j<size;j++){                                  // this loop goes through the whole line and highlights all patten matches in this line
            int match = 0;                                          
            if(j<size-p+1)                                          // if there is some hope of matching the patten before the string ends
            for(int k = 0;k<p;k++)if(line[j+k]==pattern[k])match++; // for all matches increment
            if(match==p){                                           // if the complete word matched
                printf("\033[1;31m%s\033[0m",pattern);              // highlight it and print
                j+=p-1;                                             // go to the character which is next to the pattern in the line
            }
            else printf("%c",line[j]);                              // if no match, then just print character by character as usual
        }
    }
}
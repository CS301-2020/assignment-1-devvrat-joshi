#include "dependencies.h"                    // dependencies

int main(int argc, char *argv[]){            // main function starts here
    for(int i=1;i<argc-1;i++){               // go through all the arguments which are converted to folders
        char folder_path[1000];              // path of folder
        int x = mkdir(argv[i], S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);                // make folder with RWX for user, RWX for group, RX for others
        if(x==0)printf("\033[1;36mDirectory Created at: %s\n\033[0m",argv[i]);  // if folder created successfully
        else printf("\033[1;31mmkdir: Failed Creating Directory : %s\n\033[0m",argv[i]); // if it fails
    }
    return EXIT_SUCCESS;
}
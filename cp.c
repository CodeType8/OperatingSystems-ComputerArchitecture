#include "util.c"

#define BUFF_SIZE 128

int main(int argc, char *argv[]){
    int success = 0;

    if(argc > 3){
        printf("Error: Too many arguements entered\n\r");

        return -1;
    }
    else if (argc < 3){
        printf("Error: Too less arguements entered\n\r");

        return -1;
    }
    //printf("file1: %s, file2: %s\n\r", argv[1], argv[2]);
    int source = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_WRONLY | O_CREAT);

    if(source < 1 || dest < 1){
        printf("Error: Unable to open files\n\r");
        close(source);
        close(dest);

        return -1;
    }


    success = cp(source, dest);

    close(source);
    close(dest);

    return success;
}

int cp(int source, int dest){
    char buf[BUFF_SIZE];
    int n = 0;
    
    while(n = read(source, buf, BUFF_SIZE)){
        write(dest, buf, n);
    }
    return 0;
}
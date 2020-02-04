#include "util.c"

#define BUFF_SIZE 128

int main(int argc, char *argv[]){
    int n = 0;
    char buf[BUFF_SIZE];

    if(argc > 2){
        printf("Error: Too many arguements entered\n\r");
        return -1;
    }

    if(argc == 2){
        int file = open(argv[1], O_RDONLY);

        if(file < 1){
            printf("Error: Unable to open file\n\r");
            close(file);

            return -1;
        }
        while(n = readline(file, buf)){
            printf("%s", buf);
            write(2, "\r", 1);
        }

        close(file);
    }
    else{
        while(n = read(0, buf, BUFF_SIZE - 1)){
            buf[n] = 0;

            printf("%s", buf);
        }
    }
    
    return 0;
}
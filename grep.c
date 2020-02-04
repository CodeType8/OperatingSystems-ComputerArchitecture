#include "util.c"

#define BUFF_SIZE 1024

int main(int argc, char *argv[]){
    int fd = 0, n = 0;
    char buf[BUFF_SIZE];

    if(argc > 3){
        printf("Error: Too many arguements entered\n\r");

        return -1;
    }

    if(argc < 2){
        printf("Error: Too few arguements entered\n\r");
    }

    if(argc == 3){ // from file
        fd = open(argv[2], O_RDONLY);

        if(fd < 1){
            printf("Error: Unable to open file\n\r");
            close(fd);

            return -1;
        }

        while(n = readline(fd, buf)){
            if(n && grep(buf, n, argv[1])){
                printf("%s\r", buf);
            }
        }
    }

    else if(argc == 2){ //from stdin
        while(n = readline(0, buf)){
            //printf("buf : %s", buf);
            if(n && grep(buf, n, argv[1])){
                printf("%s", buf);
                write(2, "\r", 1);
            }
        }
    }
}

int grep(char * line, int length, char *needle){
    int i = 0;
    
    for(i = 0; i < length; i++){
        if(line[i] == *needle){       
            if(strncmp(line + i, needle, strlen(needle)) == 0){
                return 1;
            }
        }
    }
    return 0;
}
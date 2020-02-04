#include "util.c"

#define BUFF_SIZE 128

int main(int argc, char * argv[]){
    if(argc > 3){
        printf("Error: Too many arguements entered\n\r");

        return -1;
    }
    else if(argc == 2){
        printf("Error: Too few arguements entered\n\r");

        return -1;
    }


    if(argc == 3){
        int source = open(argv[1], O_RDONLY);
        int dest = open(argv[2], O_WRONLY | O_CREAT);

        if(source < 1 || dest < 1){
            printf("Error: Unable to open files\n\r");
            close(source);
            close(dest);

            return -1;
        }

        l2ufiles(source, dest);
        close(source);
        close(dest);
    }
    else{
        char buf[BUFF_SIZE];
        int n = 0;

        while(n = readline(0, buf)){
            l2ubuff(buf, n - 1);
            printf("%s", buf);
            write(2, "\r", 1);
        }
    }
    
    return 0;
}

int l2ufiles(int source, int dest){
    char buf[BUFF_SIZE];
    int n = 0;

    while(n = readline(source, buf)){
        l2ubuff(buf, n - 1);
        write(dest, buf, n);
    }

    return 0;
}

int l2ubuff(char *buf, int size){
    int i = 0;

    for(i = 0 ; i < size; i++){
        if(buf[i] >= 'a' && buf[i] <= 'z'){
            buf[i] += ('A' - 'a');
        }
    }

    return 0;
}
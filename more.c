#include "util.c"

#define BUFFF_SIZE 128
#define MAX_LINE_COUNT 24

#define NEW_LINE 1
#define NEW_SCREEN 2

int main(int argc, char * argv[]){
    int n = 0, i = 0;
    char buf[BUFFF_SIZE];

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
            if(i >= MAX_LINE_COUNT){
                char input = getc();

                if(input == ' '){
                    i = 0;
                }
            }

            //buf[n] = 0;
            printf("%s\r", buf);
            
            i++;
        }

        close(file);
    }
    else{
        while(n = readline(0, buf)){
            if(i >= MAX_LINE_COUNT){
                int pid = fork(), status;
                
                if(pid < 1){
                    exit(1);
                }
                if(pid)
                    wait(&status);
                else
                    child();
                if(status == NEW_SCREEN)
                    i = 0;
            }

            buf[n] = 0;

            printf("%s\r", buf);

            i++;
        }
    }
}

int child(){
    char tty[BUFFF_SIZE], input;
    
    close(0);
    gettty(tty);
    open(tty, O_RDONLY);

    input = getc();

    if(input == ' ')
        exit(NEW_SCREEN);
    else
        exit(NEW_LINE);
}

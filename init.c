#include "ucode.c"

int console;
int console2;
int console3;

int parent()
{
    int pid, status;
    while(1){
        printf("INIT: wait for ZOMBIE child\n");

        pid = wait(&status);

        // if console login process died
        if (pid==console){      
            printf("INIT: forks a new console login\n");

            // fork another one
            console = fork();

            if (console)
                continue;
            else
                exec("login /dev/tty0"); // new console login process
        }
        // if console login process died
        else if (pid==console2){ 
            printf("INIT: forks a new console login\n");

            // fork another one
            console2 = fork();

            if (console2)
                continue;
            else
                exec("login /dev/ttyS0"); // new console login process
        }
        // if console login process died
        else if (pid==console3){ 
            printf("INIT: forks a new console login\n");

            // fork another one
            console3 = fork(); 

            if (console3)
                continue;
            else
                exec("login /dev/ttyS1"); // new console login process
        }

        printf("INIT: an orphan child buried (proc %d)\n", pid);
    }
}
main()
{
    // file descriptors for terminal I/O
    int in, out;
    
    in = open("/dev/tty0", O_RDONLY);   // file descriptor 0
    out = open("/dev/tty0", O_WRONLY);  // for display to console
    
    printf("INIT : fork a login proc on console\n");
    
    console = fork();
    
    if (console){ 
        // parent
        console2 = fork();
        if(console2){
            console3 = fork();
                if(console3)
                    parent();
                else{
                    exec("login /dev/ttyS1");
                }
        }
        else{
            exec("login /dev/ttyS0");
        }
    }
    // child: exec to login on tty0
    else
        exec("login /dev/tty0");
}
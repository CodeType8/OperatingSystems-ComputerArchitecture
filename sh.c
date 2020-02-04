#include "util.c"

#define BUFFER_SIZE  128
#define MAX_INPUT_SIZE  256
#define MAX_TOKEN_NUMBER  32

char *secondaryArgv[MAX_TOKEN_NUMBER]; 
char *pipedCMDArgs[MAX_TOKEN_NUMBER]; 
int secondaryArgc;

char * getHomeDir(){
    int uid = getuid();
    int pfd = open("/etc/passwd", O_RDONLY);
    int n;
    char buf[BUFFER_SIZE], *substring;

    while(n = readline(pfd, buf))
        {
            buf[n - 1] = 0; // kill newline
            substring = strtok(buf, ':'); // username
            substring = strtok(0, ':'); // password
            substring = strtok(0, ':'); // gid
            substring = strtok(0, ':'); //uid
            substring = strtok(0, ':'); //fullname
            substring = strtok(0, ':'); // homedir

            return substring;
        }
}

char * getName(){
    int uid = getuid();
    int pfd = open("/etc/passwd", O_RDONLY);
    int n;
    char buf[BUFFER_SIZE], *substring;

    while(n = readline(pfd, buf))
        {
            buf[n - 1] = 0; // kill newline
            substring = strtok(buf, ':'); // username

            return substring;
        }
}

void runCommand(char ** args, int numArgs)
{
    char buffer[BUFFER_SIZE];
    int i;

    setIORedirects(args, numArgs);

    strcpy(buffer, args[0]);

    for(i = 1; i < numArgs && args[i] != 0; i++){
        strcat(buffer, " ");
        strcat(buffer, args[i]);
    }

    exec(buffer); // changes image to new command
    exit(1);
}

int forkAndWait()
{
    int pid, status;

    pid = fork();

    if (pid < 0){   // fork() may fail. e.g. no more PROC in Kernel
        //perror("fork failed");
        printf("There has no proccesses anymore\n\r");
        exit(1);
    }
    if(pid) //parent
    {
        pid=wait(&status); // wait for child to die
        // while(pipedKids > 0)
        // {
        //     pid=wait(&status);
        //     pipedKids--;
        // }
        printf("DEAD CHILD=%d, HOW=%x\n\r", pid, status);
    }
    else // child
    {
        pipeHandling(secondaryArgv, secondaryArgc);
        //runCommand(secondaryArgv, secondaryArgc);
        exit(0); // exits child after its done
    }
    return status;
}

void cd()
{
    char HOME[BUFFER_SIZE];

    if(secondaryArgc > 1) // passed with an arguement
    {
        chdir(secondaryArgv[1]);
    }
    else
    {
        //strcpy(HOME, getHomeDir());
        chdir(getHomeDir());
    }
}

void clearargs()
{
    int i;

    for(i = 0; i < secondaryArgc; i++)
    {
        secondaryArgv[i] = 0;
        pipedCMDArgs[i] = 0;
    }

    secondaryArgc = 0;
    
}

void setIORedirects(char ** args, int numArgs)
{    
    int i;
    
    for(i = numArgs - 1; i > 0; i--)
    {
        if(strcmp(args[i], "<") == 0) // input from file
        {
            //printf("closing std in\n\r");
            close(0);
            open(args[i+1], O_RDONLY);

            args[i] = 0;
        }
        else if(strcmp(args[i], ">") == 0)// output to file
        {
            //printf("closing std out\n\r");
            close(1);
            open(args[i+1], O_WRONLY|O_CREAT);

            args[i] = 0;
        }
        else if(strcmp(args[i], ">>") == 0) //append output to file
        {
            //printf("closing std out\n\r");
            close(1);
            open(args[i+1], O_WRONLY|O_APPEND|O_CREAT);

            args[i] = 0;
        }
    }
}

void pipeHandling(char ** args, int numArgs)
{
    int i, pid, j;
    int pd[2];
    char ** secondCMD;

    secondCMD = NULL;

    for(i = 0; i < numArgs; i++)
    {
        if(strcmp(args[i], "|") == 0) // are equal
        {
            secondCMD = args + i;
            *secondCMD = 0;
            secondCMD++;

            break;
        }
    }

    if(secondCMD != NULL) // there is a pipe
    {
        j = 0;

        while(args + j + 1 < secondCMD )
        {
            pipedCMDArgs[j] = args[j];
            j++;
        }

        pipe(pd); // create pipe

        pid = fork();

        if (pid < 0){   // fork() may fail. e.g. no more PROC in Kernel
            //perror("fork failed");
            exit(1);
        }

        if(pid) // parent, reader
        {
            close(pd[1]); // READER MUST close pd[1]

            close(0);  
            dup(pd[0]);   // replace 0 with pd[0]
            close(pd[0]); // close pd[0] since it has replaced 0
            pipeHandling(secondCMD, numArgs - i - 1);
            //runCommand(secondCMD, numArgs - i - 1);
        }
        else // child, writer
        {
            close(pd[0]); // WRITER MUST close pd[0]

            close(1);     // close 1
            dup(pd[1]);   // replace 1 with pd[1]
            close(pd[1]); // close pd[1] since it has replaced 1
            runCommand(pipedCMDArgs, j);   // change image to cmd1
        }
    }
    else //no pipe
    {
        runCommand(args, numArgs);
    }
}

int main()
{
    char CWD[BUFFER_SIZE];
    int status;
    int i = 0;
    char name[128];

    strcpy(name, getName());

    while(1) // runs main loop until exit command input
    {
        char line[BUFFER_SIZE];

        getcwd(CWD);

        printf("SH:%s:%s", name, CWD);

        int n = gets(line);

        secondaryArgc = tokenize(line, secondaryArgv, ' ');

        if(strcmp(secondaryArgv[0], "cd") == 0) // are equal
        {
            cd();
        }
        else if(strcmp(secondaryArgv[0], "exit") == 0 || 
            strcmp(secondaryArgv[0], "logout") == 0) //are equal
        {
            exit(1);
        }
        else if (strcmp(secondaryArgv[0], " ") == 0 ||
            strcmp(secondaryArgv[0], "") == 0){
                
            continue;
        }
        else
        {
            forkAndWait();
        }
        clearargs();
    }
    return 0;
}
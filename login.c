#include "util.c"

int in, out, err, n = 0;
char username[128], password[128], buf[128];
char * substring = 0;

// login.c : Upon entry, argv[0]=login, argv[1]=/dev/ttyX
main(int argc, char *argv[])
{
    // close file descriptors 0 and 1 from init
    close(0);
    close(1);

    // open argv for in, out and err
    in = open(argv[1], O_RDONLY);
    out = open(argv[1], O_WRONLY);
    err = open(argv[1], O_WRONLY);

    // set tty to input tty
    settty(argv[1]);

    while(1){
        int pfd = open("/etc/passwd", O_RDONLY);

        printf("Login on OS - Yongmin Joh\n");
        printf("ID: "); gets(username);
        printf("password: "); gets(password);

        while(n = readline(pfd, buf))
        {
            buf[n - 1] = 0;     // kill newline
            substring = strtok(buf, ':');   // username

            if(strcmp(substring, username) == 0){
            //if(strcmp("asd", username) == 0){
                substring = strtok(0, ':');     // password

                if(strcmp(substring, password) == 0){
                //if(strcmp("asd", password) == 0){
                    printf("==============================================================\n");
                    printf("\n\rWelcome to OS of Yongmin Joh %s\n\r", username);
                    printf("==============================================================\n");

                    substring = strtok(0, ':');     // gid

                    int gid = strToInt(substring);

                    substring = strtok(0, ':');     //uid

                    int uid = strToInt(substring);

                    chuid(uid, gid);
                    
                    substring = strtok(0, ':');     //fullname
                    substring = strtok(0, ':');     // homedir

                    //printf("home : %s\n\r", substring);
                    chdir(substring);

                    substring = strtok(0, ':');     // program
                    
                    printf("%s", substring);
                    
                    exec(substring);
                }
            }
        }
        printf("login failed, Please enter correctly\n\r");
    }

}

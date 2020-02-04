#include "util.c"

#define BUFF_SIZE 1024

int main(int argc, char *argv[]){
    char old[BUFF_SIZE];

    getcwd(old);
    
    if(argc > 2){
        printf("Error: Too many arguements entered\n\r");

        return -1;
    }

    if(argc == 2){ // ls file or dir
        int success = 0;
        //int file = open(argv[1], O_RDONLY);
        char buf[BUFF_SIZE];

        success = stat(argv[1], buf);
        STAT *statPtr = (STAT*) buf;

        if(success == -1){
            printf("Error: Unable to stat file\n\r");

            return -1;
        }
        if(statPtr->st_mode & 0100000){
            lsfile(argv[1]);
        }
        else if(statPtr->st_mode & 040000){
            lsdir(argv[1]);
        }
        else{
            printf("Error: Unsupported ls file type\n\r");

            return -1;
        }
        
    } 
    else{ // ls cwd
        char name[BUFF_SIZE];

        getcwd(name);
        lsdir(name);
    }
    chdir(old);
}

void permisionPrint(int mode){
    char permission[11] = "----------\0";

    if(0120000 & mode) permission[0] = 'l';
    if(040000 & mode) permission[0] = 'd';
    if(0100000 & mode) permission[0] = '-';
    if(0400 & mode) permission[1] = 'r';
    if(0200 & mode) permission[2] = 'w';
    if(0100 & mode) permission[3] = 'x';
    if(040 & mode) permission[4] = 'r';
    if(020 & mode) permission[5] = 'w';
    if(010 & mode) permission[6] = 'x';
    if(04 & mode) permission[7] = 'r';
    if(02 & mode) permission[8] = 'w';
    if(01 & mode) permission[9] = 'x';
    printf("%s ", permission);
}



int lsfile(char * name){
    char buf[BUFF_SIZE];
    int success = stat(name, buf);

    STAT *statPtr = (STAT*) buf;

    permisionPrint(statPtr->st_mode);
    printf("%u ", statPtr->st_nlink);
    printf("%u ", statPtr->st_uid);
    printf("%u ", statPtr->st_gid);

    printf(" %d", statPtr->st_size);
    printf(" %s\n\r", name);
    
}

int lsdir(char * name){
    int fd = open(name, O_RDONLY);
    char buf[BUFF_SIZE], otherbuf[BUFF_SIZE], *cp = 0, temp[BUFF_SIZE];
    int success = fstat(fd, buf);

    DIR * dp = 0;
    STAT *statPtr = (STAT*) buf;

    if(success == -1){
        printf("Error: Unable to stat file\n\r");

        return -1;
    }

    chdir(name);
    read(fd, otherbuf, BUFF_SIZE);

    dp = cp = otherbuf;

    while(cp < otherbuf + BUFF_SIZE){
        strcpy(temp, dp->name);

        temp[dp->name_len] = 0;
        
        lsfile(temp);

        cp += dp->rec_len;
        dp = (DIR *)cp;
    }
}
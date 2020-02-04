#include "ucode.c"

#define NULL 0

char * lastPassedString = NULL;

char *strtok(char *string, char delimiter)
{
    char *current = NULL;
    if(string){ // a strng was passed in
        lastPassedString = string;
    }
    else if(lastPassedString == NULL) // unless it was null
            return NULL;


    current = lastPassedString;

    while (*lastPassedString != delimiter && *lastPassedString != '\0')
    {
        lastPassedString++;
    }


    if(*lastPassedString == '\0'){ // this string is done for
        lastPassedString = NULL;
    }
    else{ // string has potentially more deliminators in it
        *lastPassedString = '\0';
        lastPassedString++;
    }

    return current;
}

// pass in a copy or line will be destroyed
// tokenizes line by the dliinator into the output array, returns the number of tokens
int tokenize(char *line, char** outputArray, char delim)
{
    char *s;
    int i;

    s = strtok(line, delim); // first call to strtok()
    i = 0;

    while(s){
        outputArray[i] = s;
        s = strtok(NULL, delim); // call strtok() until it returns NULL
        i++;
    }
    return i;
}

int strToInt(char *string)
{
    int num = 0;
    while (*string != '\0')
    {
        num *= 10;
        num += (int)((char)*string - '0');
        string++;
    }

    return num;
}

int readline(int fd, char *s)
{
    char c;
    char *cp = s;
    int result = 1;

    result = read(fd, &c, 1);

    while (result != 0 && (c != EOF) && (c != '\r') && (c != '\n'))
    {
        *cp++ = c;
        result = read(fd, &c, 1);
    }

    if (c == EOF)
        return 0;
    if (result == 0)
        return 0;

    *cp++ = c; // a string with last char=\n or \r
    *cp = 0;
    
    //printf("getline: %s", s);

    return strlen(s); // at least 1 because last char=\r or \n
}

//replaces /n /r with nulls
void normalize(char *string)
{
    replace(string, '\r', '\0');
    replace(string, '\n', '\0');
}

// replace all instances of a character in a string with another character;
void replace(char *string, char character, char replacement)
{
    while (*string != '\0')
    {
        if (*string == character)
        {
            *string = replacement;
        }

        string++;
    }
}
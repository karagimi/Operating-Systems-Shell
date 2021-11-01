/*
    @author: Michalis Karagiannakis csd4355
*/

/*pipes*/
#define _GNU_SOURCE
/*#include <fnct1.h>*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /*fork*/
#include <signal.h>

#include <sys/wait.h>
#include <sys/types.h> /*fork*/

#define TRUE 1
#define SIZE 1000

/*Prints the prompt to get a new command*/
void type_prompt();

/*Reads the command*/
void read_command(char*);

int main() {
    char *input;
   /* while(TRUE) {*/
        type_prompt();
        read_command(input);

        if(fork()!=0) {
            /*Parent code*/
            /* waitpid(-1,&status,0);*/
        }else {
            /*Child code*/
            /*execve(command,parameters,0);*/
        }
    }
}

/*Prints the prompt to get a new command*/
void type_prompt() {
    char dir[SIZE];
    char *user;

    user=getlogin();
    if(!user) {
        perror("getlogin() error");
    }
    getcwd(dir,SIZE);

    printf("%s",user);
    printf("@");
    printf("cs345sh");
    printf("%s",dir);
    printf("$");
    printf("\n");
}

void read_command(char *input) {
    int ch,count=0;
    while((ch=getchar())!='\n') {
        if(count==SIZE) {
            break;
        }
        input[count]=ch;
        count++;
    }
    printf("%s",input);
}
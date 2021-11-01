/*
    @author: Michalis Karagiannakis csd4355
*/

/*pipes*/
#define _GNU_SOURCE
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /*fork*/
#include <signal.h>

#include <sys/wait.h>
#include <sys/types.h> /*fork*/

#define TRUE 1
#define SIZE 100
#define MAX_ARGS 100

/*Prints the prompt to get a new command*/
void type_prompt();

int main() {
    char input[SIZE];
    int status;
    char* argv[MAX_ARGS];
    while(TRUE) {
        type_prompt();
        argv[0]="";
        argv[1]="";

        int ch,count=0;
        while((ch=getchar())!='\n') {
            if(count==SIZE) {
                break;
            }
            input[count]=ch;
            count++;
        }
        if(strcmp(input,"ls")==0) {
            argv[0]="/bin/ls";
            argv[1]=NULL;
        }

        if(strcmp(input,"exit")==0) {
            exit(0);
        }

        pid_t pid=fork();

        if(pid==-1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }else if(pid==0) {
             /*Child code*/
            if (execv(argv[0], argv) < 0) {
                printf("\nCould not execute command..");
            }
            exit(0);
        }else {
            /*Parent code*/
            wait(&status);
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
    printf("$ ");
}


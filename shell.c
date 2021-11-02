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
    char *user;
    char *token;
    while(TRUE) {
        type_prompt();
        int i=0;
        int ch,count=0;

        memset(input,0,strlen(input));
        argv[0]=NULL;
        argv[1]=NULL;

        while((ch=getchar())!='\n') {
            if(count==SIZE) {
                break;
            }
            input[count]=ch;
            count++;
        }

        token=strtok(input," ");        /*ls*/
        if(strcmp(token,"ls")==0) {
            argv[0]="/bin/ls";
            token=strtok(NULL," ");
            printf("%s",token);
            argv[1]=token;
        }

        if(strcmp(input,"exit")==0) {   /*exit*/
            exit(0);
        }

        if(strcmp(token,"cd")==0) {     /*cd*/
        printf("%ld",strlen(input));
            if(strlen(input)!=2) {
                token=strtok(NULL," ");
            }

            if(strlen(input)==2) {
                chdir("/");
            }
            argv[0]="/bin/chdir";
            argv[1]=token;
        }

        if(strcmp(input,"pwd")==0) {    /*pwd*/
            argv[0]="/bin/pwd";
            argv[1]=NULL;
        }

        if(strcmp(input,"whoami")==0) { /*whoami*/
            user=getlogin();
            printf("%s",user);
        }

        pid_t pid=fork();

        if(pid==-1) {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }else if(pid==0) {
             /*Child code*/
            execv(argv[0], argv);
            printf("\nCould not execute command..");
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

void fork_proccess(char *input,char **args) {

}
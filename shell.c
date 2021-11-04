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

char **get_command(char *input);

char **get_sequential_command(char *input);

void remove_spaces(char *input);

int main() {
    char input[SIZE];
    char *argv[SIZE];
    char **command;
    char *token;
    char *user;
    int index;
    int count=0;
    int command_count=0;
    int command_index=0;
    while(TRUE) {

        type_prompt();
        int i=0;
        int ch,count=0;

        while((ch=getchar())!='\n') {
            if(count==SIZE) {
                break;
            }
            input[count]=ch;
            count++;
        }
        input[count]='\0';
        if(strchr(input,';')==NULL) { 
            command=get_command(input);
            if(strcmp(command[0],"cd") == 0 ){
                if(command[1]==NULL) {
                    chdir("/");
                }else{
                    chdir(command[1]);
                }
            }

            if(strcmp(command[0],"exit")==0) {
                exit(0);
            }

            pid_t pid=fork();

            if(pid==-1) {
                perror("Fork error");
                exit(EXIT_FAILURE);
            }else if(pid==0) {
                /*Child code*/
                execvp(command[0], command);
                printf("\nCould not execute command..\n");
                exit(0);
            }else {
                /*Parent code*/
                wait(NULL);
            }
        }else {
            
            index=0;
            command_count=0;
            command_index=0;
            command=get_sequential_command(input);
            while(command[command_index]!=NULL) {
                command_count++;
                command_index++;
            }
            while(index<command_count) {
                count=0;
                pid_t pid=fork();

                if(pid==-1) {
                    perror("Fork error");
                    exit(EXIT_FAILURE);
                }else if(pid==0) {
                    /*Child code*/
                    token=strtok(command[index]," ");
                    while(token!=NULL) {
                        argv[count]=token;
                        /*printf("argv[%d] = %s\n",count,argv[count]);*/
                        token=strtok(NULL," ");
                        count++;
                    }
                    argv[count]=NULL;
                    /*printf("command[index] = %s\n",command[index]);*/
                    
                    execvp(command[index],argv);
                   /* printf("\nCould not execute command..\n");*/
                    exit(0);
                }else{
                    /*Parent code*/
                    wait(NULL);
                }
                index++;
            }
            /*free(command);*/
        }
    }
}

/*Prints the prompt to get a new command*/
void type_prompt() {
    char dir[SIZE];
    char *user;

   /* user=getlogin();*/
    user="karagimi";
    if(!user) {
      /*  perror("getlogin() error");*/
    }
    getcwd(dir,SIZE);

    printf("%s",user);
    printf("@");
    printf("cs345sh");
    printf("%s",dir);
    printf("$ ");
}

char **get_command(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input," ");
    while(parsed!=NULL) {
        command[index]=parsed;
        index++;
        parsed=strtok(NULL," ");
    }
    command[index]=NULL;
    return command;
}

char **get_sequential_command(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input,";");
    while(parsed!=NULL) {
        command[index]=parsed;
        index++;
       /* printf("parsed = %s\n",parsed);*/
        parsed=strtok(NULL,";");
        if((parsed!=NULL)) {
            remove_spaces(parsed);
        }
        printf("parsed_After_remove = %s\n",parsed);
    }
    command[index]=NULL;
    return command;
}

void remove_spaces(char *s) {
    char*d =s;
    do {
        while(*d == ' ') {
            ++d;
        }
    }while(*s++ = *d++);
}
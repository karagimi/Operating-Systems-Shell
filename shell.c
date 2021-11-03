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
    char input2[SIZE];
    int status;
    char* argv[MAX_ARGS];
    char *user;
    char *token;
    char directory[SIZE];
    int index;
    int semicolonCounter=0;
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
        input[count]='\0';
        strcpy(input2,input);

        if(strchr(input,';')==NULL) {
            pid_t pid=fork();

            if(pid==-1) {
                perror("Fork error");
                exit(EXIT_FAILURE);
            }else if(pid==0) {
                /*Child code*/
                if(strchr(input,'>')||(strchr(input,'<'))!=NULL) {
             /*   int file=open("test.txt",O_WRONLY | O_CREAT,0777);
                if(file == -1) {
                    return 0;
                }*/
            }
            token=strtok(input," "); 

            if(strcmp(token,"ls")==0) {     /*ls*/
                if(strlen(input2)==2) {
                    argv[0]="/bin/ls";
                    argv[1]=NULL;
                }else if(strlen(input2)>2){
                    argv[0]="/bin/ls";
                    token=strtok(NULL," ");
                    argv[1]=token;
                    argv[2]=NULL;
                }
            }
            
            if(strcmp(input,"exit")==0) {   /*exit*/
                exit(0);
            }

            if(strcmp(token,"cd")==0) {     /*cd*/
                if(strlen(input2)==2) {
                    /*chdir("/");*/
                }else if(strlen(input2)>2) {
                    token=strtok(NULL," ");
                    strcpy(directory,token);
                    chdir(directory);
                }
            }

            if(strcmp(input,"pwd")==0) {    /*pwd*/
                argv[0]="/bin/pwd";
                argv[1]=NULL;
            }

            if(strcmp(input,"whoami")==0) { /*whoami*/
                argv[0]="/usr/bin/whoami";
                argv[1]=NULL;
            }

            if(strcmp(input,"mkdir")==0) {
                token=strtok(NULL," ");
                argv[0]="/bin/mkdir";
                argv[1]=token;
                argv[2]=NULL;
            }

            if(strcmp(input,"rmdir")==0) {
                token=strtok(NULL," ");
                argv[0]="/bin/rmdir";
                argv[1]=token;
                argv[2]=NULL;
            }

                execv(argv[0], argv);
            /* printf("\nCould not execute command..\n");*/
                exit(0);
            }else {
                /*Parent code*/
                wait(NULL);
            }

        }else{
            for(index=0;index<strlen(input);index++) {
                if(input[index]==';') {
                    semicolonCounter++;
                }
            }
            token=strtok(input,";");
            index=0;
            for(index=0;index<semicolonCounter+1;index++) {
                pid_t pid2=fork();
                if((strcmp(token,"ls ")==0)||(strcmp(token," ls")==0)||(strcmp(token," ls ")==0)) {
                    argv[0]="/bin/ls";
                    argv[1]=NULL;
                }

                if((strcmp(token,"pwd ")==0)||(strcmp(token," pwd")==0)||(strcmp(token," pwd ")==0)) {
                    argv[0]="/bin/pwd";
                    argv[1]=NULL;
                }

                if((strcmp(token,"whoami ")==0)||(strcmp(token," whoami")==0)||(strcmp(token," whoami ")==0)) { /*whoami*/
                    argv[0]="/usr/bin/whoami";
                    argv[1]=NULL;
                }

                if((strcmp(token,"exit ")==0)||(strcmp(token," exit")==0)||(strcmp(token," exit ")==0)) {
                    exit(0);
                }

                if(pid2==-1) {
                    perror("Fork error");
                    exit(EXIT_FAILURE);
                }else if(pid2==0) {
                    /*printf("token1 = %s\n",token);*/
                    execv(argv[0], argv);
                    /*printf("\nCould not execute command..\n");*/
                    exit(0);
                }else {
                    /*Parent code*/
                    wait(NULL);
                    /*printf("returned to parent\n");*/
                }
                token=strtok(NULL,";");
                if(token==NULL) {
                    break;
                }
                /*  printf("token2 = %s\n",token);*/
            }
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
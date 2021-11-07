/*
    @author: Michalis Karagiannakis csd4355@csd.uoc.gr
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

char **get_redirection_command_out(char *input);

char **get_redirection_command_out_double(char *input);

char **get_redirection_command_in(char *input);

char **get_pipe1_command(char *input);

char **get_pipe2_command(char *input);

char **get_multipipe_command(char *input);

int main() {
    char input[SIZE];
    char input2[SIZE];
    char *argv[SIZE];
    char **command;
    char **command2;
    char *token;
    char *user;
    int index;
    int count=0;
    int command_count=0;
    int command_index=0;
    int redirection_count;
    int pipe_count;
    int i=0;
    int count_pipe_command=0;
    while(TRUE) {

        type_prompt();
        int i=0;
        int ch,count=0;
        int count_pipe_command=0;

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
            if(strchr(input,'>')!=NULL || strchr(input,'<')!=NULL) {    /*Redirections*/
                if(strchr(input,'>')!=NULL && strchr(input,'<')==NULL) {    /*>*/
                    redirection_count=0;
                    for(i=0;i<strlen(input);i++) {
                        if(input[i]=='>') {
                            redirection_count++;
                        }
                    }

                    if(redirection_count==1) {  /*>*/
                        command=get_redirection_command_out(input);
                        pid_t pidred=fork();

                        if(pidred==-1) {
                            perror("Fork error");
                            exit(EXIT_FAILURE);
                        }else if(pidred==0) {
                            /*Child code*/
                            int command_count2=0;
                            int command_index2=0;
                            while(command[command_index2]!=NULL) {
                                command_count2++;
                                command_index2++;
                            }

                            int file=open(command[command_index2-1],O_WRONLY | O_CREAT | O_TRUNC,0777);
                            if(file==-1) {
                                return 0;
                            }

                            command[command_index2-1]=NULL;

                            int file2=dup2(file,STDOUT_FILENO);
                            close(file);

                            execvp(command[0], command);
                           /* printf("\nCould not execute command..\n");*/
                            exit(0);
                        }else{
                            /*Parent code*/
                            wait(NULL);
                        }
                    }else if(redirection_count==2) {        /*>>*/
                        command=get_redirection_command_out_double(input);
                        pid_t pidapp=fork();

                        if(pidapp==-1) {
                            perror("Fork error");
                            exit(EXIT_FAILURE);
                        }else if(pidapp==0) {

                            int command_count3=0;
                            int command_index3=0;
                            while(command[command_index3]!=NULL) {
                                command_count3++;
                                command_index3++;
                            }
                            

                            int file=open(command[command_index3-1],O_WRONLY | O_CREAT | O_APPEND,0777);
                            if(file==-1) {
                                return 0;
                            }

                            command[command_index3-1]=NULL;

                            int file2=dup2(file,STDOUT_FILENO);
                            close(file);

                            execvp(command[0], command);
                            printf("\nCould not execute command..\n");
                            exit(0);
                        }else{
                            wait(NULL);
                        }
                    }
                }else if(strchr(input,'<')!=NULL && strchr(input,'>')==NULL) {  /*<*/
                    command=get_redirection_command_in(input);
                    pid_t pidred2=fork();

                    if(pidred2==-1) {
                        perror("Fork error");
                        exit(EXIT_FAILURE);
                    }else if(pidred2==0) {
                        /*Child code*/

                        int file=open(command[1],O_RDONLY);
                        if(file==-1) {
                            printf("The file does not exist\n");
                            return 0;
                        }

                        int file2=dup2(file,0);
                        close(file);

                        command[1]=NULL;

                        execvp(command[0], command);
                        printf("\nCould not execute command..\n");
                        exit(0);
                    }else{
                        /*Parent code*/
                        wait(NULL);
                    }
                }

            }else{
                if(strchr(input,'|')!=NULL) {   /* PIPES (=1) */
                    pipe_count=0;

                    for(i=0;i<strlen(input);i++) {
                        if(input[i]=='|') {
                            pipe_count++;
                        }
                    }

                    if(pipe_count==1) {
                        int pipefd[2];
                        if(pipe(pipefd)==-1) {
                            return -1;
                        }

                        command=get_pipe1_command(input);

                        int pidpipe = fork();

                        if(pidpipe<0) {
                            printf("Fork Error\n");
                            return 0;
                        }else if(pidpipe==0) {
                            //Child Process 1

                            dup2(pipefd[1],STDOUT_FILENO);
                            close(pipefd[0]);
                            close(pipefd[1]);
                            execvp(command[0],command);
                        }

                        command2=get_pipe2_command(input2);

                        int pidpipe2 = fork();
                        if(pidpipe2 < 0) {
                            return 3;
                        }else if(pidpipe2==0) {
                            //Child Process 2
                            dup2(pipefd[0],STDIN_FILENO);
                            close(pipefd[0]);
                            close(pipefd[1]);
                            execvp(command2[0],command2);
                        }

                        close(pipefd[0]);
                        close(pipefd[1]);

                        waitpid(pidpipe,NULL,0);
                        waitpid(pidpipe2,NULL,0);
                    }else{
                        command=get_multipipe_command(input);

                        int fd[pipe_count][2];
                        int i=0;
                        int commands_number=pipe_count+1;
                        pid_t pid;
                        index=0;

                        for(i=0;i<pipe_count;i++) {
                            if(pipe(fd[i])<0) {
                                perror("Pipe error");
                                exit(EXIT_FAILURE);
                            }
                        }


                        while(index<commands_number) {
                            int count=0;
                            pid_t pid = fork();
                            if(pid==-1) {
                                perror("Fork error");
                                exit(EXIT_FAILURE);
                            }else if(pid==0) {
                                token=strtok(command[index]," ");
                                printf("command[%d] = %s\n",index,command[index]);
                                while(token!=NULL) {
                                    token=strtok(NULL," ");
                                    argv[count]=token;
                                    count++;
                                }
                                argv[count]=NULL;

                                if(index!=commands_number) {
                                    if(dup2(fd[index][1],STDOUT_FILENO)<0) {
                                        perror("Dup2 error");
                                        exit(EXIT_FAILURE);
                                    }
                                }

                                if(index!=0) {
                                    if(dup2(fd[index][0],STDIN_FILENO)<0) {
                                        perror("Dup2 error");
                                        exit(EXIT_FAILURE);
                                    }
                                }

                                execvp(command[index],argv);
                            }else{
                                wait(NULL);
                                
                            }
                            index++;
                        }
        
                    }

                }else {
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
                }
            }
        }else{
            
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
                        token=strtok(NULL," ");
                        count++;
                    }
                    argv[count]=NULL;
            
                    execvp(command[index],argv);
                    printf("\nCould not execute command..\n");
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
     //   printf("command[%d] = %s\n",index,command[index]);
        index++;
        parsed=strtok(NULL,";");
        if(parsed!=NULL) {
            remove_spaces(parsed);
        }
    }
    command[index]=NULL;
    return command;
}

void remove_spaces(char *s) {
    char*d =s;
    int i=0;
    int count=0;
    for(i=0;i<strlen(d);i++) {
        if(d[i]==' ') {
            count++;
        }
    }
    if(count==1) {
        do {
            while(*d == ' ') {
                ++d;
            }
        }while(*s++ = *d++);
    }else if(count>1){
        for(i=0;i<strlen(d);i++) {
            d[i]=d[i+1];
        }
    }
}


char **get_redirection_command_out(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input," ");
    while(parsed!=NULL) {
        if(strchr(parsed,'>')!=NULL) {
            parsed=strtok(NULL," ");
            break;
        }
        command[index]=parsed;
        index++;
        if((parsed!=NULL)) {
            remove_spaces(parsed);
        }
        parsed=strtok(NULL," ");
    }
    command[index]=parsed;
    command[index+1]=NULL;
    return command;
}

char **get_redirection_command_out_double(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input," ");
    while(parsed!=NULL) {
        if(strcmp(parsed,">>")==0) {
            parsed=strtok(NULL," ");
            break;
        }
        command[index]=parsed;
        index++;
        if((parsed!=NULL)) {
            remove_spaces(parsed);
        }
        parsed=strtok(NULL," ");
    }
    command[index]=parsed;
    command[index+1]=NULL;
    return command;
}

char **get_redirection_command_in(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input," ");
    while(parsed!=NULL) {
        if(strchr(parsed,'<')!=NULL) {
            parsed=strtok(NULL," ");
            break;
        }
        command[index]=parsed;
        index++;
        if((parsed!=NULL)) {
            remove_spaces(parsed);
        }
        parsed=strtok(NULL," ");
    }
    command[index]=parsed;
    command[index+1]=NULL;
    return command;
}

char **get_pipe1_command(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input," ");
    while(strchr(parsed,'|')==NULL) {
        command[index]=parsed;
        printf("%s\n",command[index]);
        index++;
        parsed=strtok(NULL," ");
    }

    command[index]=NULL;
    return command;
}

char **get_pipe2_command(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input,"|");
    while(parsed!=NULL) {
        parsed=strtok(NULL," ");
        command[index]=parsed;
        index++;
    }
    command[index]=NULL;
    return command;
}

char **get_multipipe_command(char *input) {
    char **command=malloc(100*sizeof(char *));
    char *parsed;
    int index=0;
    parsed=strtok(input,"|");
    while(parsed!=NULL) {
        command[index]=parsed; 
        parsed=strtok(NULL,"|");
        if((parsed!=NULL)) {
            remove_spaces(parsed);
        }
        index++;
    }
    command[index]=NULL;
    return command;
}

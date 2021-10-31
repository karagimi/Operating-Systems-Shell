/*
    @author: Michalis Karagiannakis csd4355
*/

/*pipes*/
#define _GNU_SOURCE
#include <fnct1.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /*fork*/
#include <signal.h>

#include <sys/wait.h>
#include <sys/types.h> /*fork*/

#define TRUE 1

/*Prints the prompt to get a new command*/
void type_prompt();

int main() {
    while(TRUE) {
        type_prompt();
        read_command(command,parameters);

        if(fork()!=0) {
            /*Parent code*/
            waitpid(-1,&status,0);
        }else {
            /*Child code*/
            execve(command,parameters,0);
        }
    }
}

/*Prints the prompt to get a new command*/
void type_prompt() {

}
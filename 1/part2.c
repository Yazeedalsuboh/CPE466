#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myshell(char *command) {
  
    if (strcmp(command, "clear\n") == 0) {
        system("clear");
    } else if (strcmp(command, "exit\n") == 0) {
        exit(0);
    } else if (strcmp(command, "listen\n") == 0) {
        system("env");
    } else if (strcmp(command, "pwd\n") == 0) {
        system("pwd");
    } else {
        printf("What???? \n");
    }
}

int main(int argc, char** argv) {
    char command[50];

    while (1) {
        
        printf("1.clear: clears the screen.\n2. exit: quits the shell.\n3. listen: lists all environment strings. \n4. pwd: prints the current working directory.\n");
        printf("MyShell>");
        
        fgets(command, sizeof(command), stdin);

        myshell(command);
    }

    return 0;
}


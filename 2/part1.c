#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 100

int readline(int fd, char* buff, int nbytes) {
    int numread = 0;
    int retvalue;

    while (numread < nbytes - 1) {

        // read a character with every iteration
        retvalue = read(fd, buff + numread, 1);

        // memo 
        if ((retvalue == -1) && (errno == EINTR)) // I/O interrupt
            continue;
        if ((retvalue == 0) && (numread == 0)) // file is empty
            return 0;
        if (retvalue == 0) // finished reading the file but '\n' doesn't exist
            break;
        if (retvalue == -1) // reading the file failed
            return -1;

        numread++;

        if (buff[numread - 1] == '\n') {
            buff[numread] = '\0';
            return numread; // number of bytes read
        };
    };


    errno = EINVAL; // error --> no new line
    return -1;

};

char** makeargv(char* tmp, char delim[]){
    int cnt = 0;
    char* str = (char*)malloc(strlen(tmp)+1);
    strcpy(str,tmp);
    
    char* token = strtok(str, delim);
    while(token != NULL){
        cnt++;
        token = strtok(NULL, delim);
    }   
    cnt++;
    char** argv = malloc(sizeof(char*)*cnt);
    
    strcpy(str,tmp);
    token = strtok(str, delim);
    cnt = 0;
    while(token != NULL){
        argv[cnt] = (char*)malloc(strlen(token)+1);
        strcpy(argv[cnt],token);
        
        // Replace newline with null terminator
        int token_len = strlen(argv[cnt]);
        if (argv[cnt][token_len - 1] == '\n') {
            argv[cnt][token_len - 1] = '\0'; 
        }
        
        token = strtok(NULL, delim);
        cnt++;
    } 
    argv[cnt] = NULL;
    

    return argv;
}

void exec(char* cmd) {
	char** argv = makeargv(cmd, " ");
	
	if (fork() == 0) {
        // child executing
		if (execvp(argv[0], argv) == -1) {
			printf("Failed to execute command\n");
		}
	} else {
        // parent waiting
		wait(NULL);
	}
};	



void main(int argc, char** argv) {
	    
    int fd = open("commands.txt", O_RDONLY);
    	
    char buff[SIZE];
    	
    while (readline(fd, buff, SIZE) > 0) {
        printf("Line read: %s", buff);
        exec(buff);
        printf("__________________________________________\n");
    };
    
    
    close(fd);	
};

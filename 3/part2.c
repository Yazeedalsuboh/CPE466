#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include<string.h>
#include<stdlib.h>



char** makeargv(char* tmp, char delim[]){
    int cnt = 0;
    char* str = (char*)malloc(strlen(tmp)+1);
    strcpy(str,tmp);
    printf("[%s]\n", str);
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
        token = strtok(NULL, delim);
        cnt++;
    } 

   
    argv[cnt]=NULL;
    return argv;
}

void exec(char* cmd, int i, int length, int parr[][2]) {
    char** argv = makeargv(cmd, " ");

    if (fork() == 0) {
        printf("child: %s, i:%i\n", argv[0], i);
        
        if (i == 0) {
            dup2(parr[i][1],1);
            close(parr[i][0]); 
        } else if (i == length) {
	    dup2(parr[i-1][0],0);
            close(parr[i-1][1]); 
        } else {
            dup2(parr[i-1][0],0);
            close(parr[i-1][1]);
            
            dup2(parr[i][1],1);
            close(parr[i][0]); 
        }
        
        
	execvp(argv[0], argv);
	
    } else {
        if (i == 0) {
            close(parr[i][1]); // close write end in parent after child is forked
        } else if ( i == length) {
        	close(parr[i-1][0]);
        }else {
            close(parr[i-1][0]); // close read end in parent after child is forked
            close(parr[i][1]);
        } 

        wait(NULL);
        wait(NULL);

    }
}




void main(int argc, char** argv){

    // Check argument number
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return;
    }
    
    // Tokenization
    char** ptr = makeargv(argv[1], "|");
    
    // Get the length of ptr
    int length = 0;
    while (ptr[length] != NULL) {
    	length++;
    }
    printf("Length: %i\n", length);
    
    // Make pipes    
    int p[length-1][2];
    for(int i = 0; i < (length - 1); i++){
	int rv = pipe(p[i]);
	printf("MADE PIPE\n");
	if(rv == -1){
		printf("Failed to create pipe %d\n", i);
		return;
	}
     }
    
    // Calling exec
    int i = 0;
    int j = 0;
    while(ptr[i] != NULL){
      
        printf("command: [%s], i: %i, j:%i\n", ptr[i], i, j);
    	  exec(ptr[i], i, length-1,p);
        i++;
    };
};
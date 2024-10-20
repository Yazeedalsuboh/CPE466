#include <stdio.h>
#include <unistd.h>
#include <signal.h>

 
void handler(int signo){
	 write(1, "Hello\n", 7);
};
 
void main(){
 	signal(SIGINT, handler);
 	
 	printf("My PID is %d\n", getpid());
	
	while(1) {
		printf("*\n");
		sleep(1);
 	};
}

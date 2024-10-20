#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int quit = 0;

void print_time(){
	time_t t = time(NULL);
	char* str = ctime(&t);
	str[strlen(str)-1]='\0';
	printf("\r%s",str);
	fflush(stdout);
}

void handler(int sig) {
	
	print_time();

};


void main() {
	signal(SIGALRM, handler);
	
	struct itimerval itv;
	
	itv.it_interval.tv_sec = 3;
	itv.it_interval.tv_usec = 0;
	
	itv.it_value.tv_sec = 3; 
	itv.it_value.tv_usec = 0;
	
	print_time(); 
	
	int it = setitimer(ITIMER_REAL, &itv, NULL);
	while(!quit);
}


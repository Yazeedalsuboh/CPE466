#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stddef.h>



void SIGINT_handler(int signo){
	

	int sig;
	
	sigset_t set; 
	sigemptyset(&set); 
	sigaddset(&set, SIGINT); 
	sigprocmask(SIG_BLOCK, &set, NULL);
	
	sigwait(&set, &sig);
};

int csig = 0;
void SIGHUP_handler(int signo) {
	
	
	csig = signo;
		
}; 



void main() {

	printf("PID = %d\n", getpid());

	signal(SIGUSR1, SIG_IGN); // a) ignores sigusr1
	
	// b) it block sigalrm
	sigset_t set; 
	sigemptyset(&set); 
	sigaddset(&set, SIGALRM); 
	sigprocmask(SIG_BLOCK, &set, NULL);

	
	// c) SIGINT stops printing numbers
	struct sigaction act_int;
	act_int.sa_handler = SIGINT_handler;
	act_int.sa_flags = 0;
	
	sigemptyset(&act_int.sa_mask);
	sigaddset(&act_int.sa_mask, SIGHUP); // block sighhup while handling
	sigaction(SIGINT, &act_int, NULL);
	
	signal(SIGINT, SIGINT_handler);


	
	// d) SIGHUP resets loop to 1
	struct sigaction act;
	act.sa_handler = SIGHUP_handler;
	act.sa_flags = 0;
	
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaction(SIGHUP, &act, NULL);
	
	signal(SIGHUP, SIGHUP_handler);

	
	for (int i = 1; i < 16; i++) {
		printf("%d\n", i);
		
		if (csig == SIGHUP) {i = 0; csig = 0;};
		
		sleep(1);
	};





};

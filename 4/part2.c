#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
    int i;
    char ch;
    
    int pid = fork();
    
    
    int sig;

    if (pid == 0) {
     
        sigset_t set;
	    sigemptyset(&set);
	    sigaddset(&set, SIGUSR1);
	    sigprocmask(SIG_BLOCK, &set, NULL);

        for (i = 1; i <= 5; i++) {
            printf("%d\n", i);
            kill(getppid(), SIGUSR1);
            sigwait(&set, &sig);
        };
    } else {
        
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);

        for (ch = 'A'; ch <= 'E'; ch++) {
            sigwait(&set, &sig);
            printf("%c\n", ch);
            kill(pid, SIGUSR1);
        }
    }

    return 0;
}



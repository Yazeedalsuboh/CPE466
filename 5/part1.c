#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY ftok("part1.c", 10)

struct message {
	long type;
	char ch;
};

int main() {
    int pid = fork();
    
    if (pid == 0) {
     	   
     	    for (int i = 0; i < 5; i++) {
		 
				int qid = msgget(KEY, IPC_CREAT|0666); // create queue
		    
				struct message m;
				m.type = 1;
				m.ch = '1';
				m.ch += i;
		    
		
				msgsnd(qid, &m, 1, 0);
		    
		 
				msgrcv(qid, &m, 1, 2, 0); // wait to revieve type 2 // 0 blocking wait
				printf("%c\n", m.ch);
     	    };
	    
	   
	    
    } else {
	
	for (int i = 0; i < 5; i++) {
		int qid = msgget(KEY, IPC_CREAT|0666);
	 	
	 	struct message m;
	    	
	    	msgrcv(qid, &m, 1, 1, 0); // wait to recieve type 1 // 0 blocking wiat
	    	
	    	printf("%c\n", m.ch);
	    	
	    	m.type = 2;
	    	m.ch = 'A';
	    	m.ch += i;
	    	
	    	msgsnd(qid, &m, 1, 0);	    	
	};
	   
    }

    return 0;
}

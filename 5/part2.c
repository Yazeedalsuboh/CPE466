#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/msg.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#define KEY ftok("part2.c", 12)

struct message {
	long type;
	char string[256]; 
};

int main() {
	while (1) {

		int qid = msgget(KEY, IPC_CREAT | 0666);
	
		if (fork() == 0) {
			// P2

			struct message m;

			msgrcv(qid, &m, sizeof(struct message) - sizeof(long), 2, 0); // wiatinf for type 2
			
			printf("P2: Received from P1 message type [%ld]: %s\n", m.type, m.string);

			// Modify string to uppercase
			for (int i = 0; m.string[i]; i++) {
				m.string[i] = toupper(m.string[i]);
			}

			m.type = 3;
			msgsnd(qid, &m, sizeof(struct message) - sizeof(long), 0);
			exit(0);
		} else if (fork() == 0) {
			// P3

			struct message m;

			msgrcv(qid, &m, sizeof(struct message) - sizeof(long), 2, 0); // waiting for type 2
			
			printf("P3: Received from P1 message type [%ld]: %s\n", m.type, m.string);

			// Modify string to lowercase
			for (int i = 0; m.string[i]; i++) {
				m.string[i] = tolower(m.string[i]);
			}

			m.type = 4;
			msgsnd(qid, &m, sizeof(struct message) - sizeof(long), 0);
			exit(0);
		} else {
			// P1
			struct message m;
			m.type = 2;

			printf("Enter a string: ");
			fgets(m.string, sizeof(m.string), stdin);


			if (strcmp(m.string, "exit\n") == 0) break;


			// send two messages of type 2
			// Send message to P2
			msgsnd(qid, &m, sizeof(struct message) - sizeof(long), 0);
			// Send message to P3
			msgsnd(qid, &m, sizeof(struct message) - sizeof(long), 0);

			printf("P1: Message Sent: %s\n", m.string);

			// Receive message from P2
			struct message m3;


			if(msgrcv(qid, &m3, sizeof(struct message) - sizeof(long), 3, 0)) {
				printf("P1: Received from P2 message type [%ld]: %s\n", m3.type, m3.string);		
			}; 

			// Receive message from P3
			struct message m4;
			if (msgrcv(qid, &m4, sizeof(struct message) - sizeof(long), 4, 0)) {
				printf("P1: Received from P3 message type [%ld]: %s\n", m4.type, m4.string);
			};

			wait(NULL);
			wait(NULL);

		};
	};

	return 0;
}


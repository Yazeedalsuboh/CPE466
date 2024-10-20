#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void main() {

	int p[2];
	pipe(p);
	
	if (fork() == 0) {
		// P2
		// A to E
		close(p[1]); // close writing end
		char ch;	
		char c = 'A';		
		while (c <= 'E') {
			read(p[0], &ch, 1);
			printf("%c\n", c);
			c++;	
		} 
	} else {
		// P1 
		// 1 to 5
		close(p[0]); // close reading end
		int x = 1;
		while (1) {
			char buff[2];
			
			buff[0] = 'a';
			
			if (x <= 6) {
				write(p[1], &buff[0], 1);
				printf("%i\n", x);
				x++;
				usleep(1000);
			};
			else break;
		}
		close(p[1]); // close writing end
		wait(NULL);
	
	}
}


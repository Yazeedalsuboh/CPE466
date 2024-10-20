#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int a[1024][1024];

void function1(){
	for(int k=0;k<10;k++) {
		for(int i=0;i<1024; i++){
			for(int j=0;j<1024; j++) {			
				a[i][j] = 0;
			};
		};
	};
};

int b[1024][1024];

void function2(){
	for(int k=0;k<10;k++) {
		for(int i=0;i<1024; i++){
			for(int j=0;j<1024; j++) {			
				b[j][i] = 0;
			};
		};
	};
};


int main() {
    struct itimerval timer1; 
    struct itimerval timer2; 

    timer1.it_interval.tv_sec = 0;
    timer1.it_interval.tv_usec = 0;
    timer1.it_value.tv_sec = 1; 
    timer1.it_value.tv_usec = 0;
   
   
    setitimer(ITIMER_VIRTUAL, &timer1, NULL);
    
    time_t i1 = timer1.it_value;

    function1();

    getitimer(ITIMER_VIRTUAL, &timer1);
    
    time_t i2 = timer1.it_value;

    
    timer2.it_interval.tv_sec = 0;
    timer2.it_interval.tv_usec = 0;
    timer2.it_value.tv_sec = 1; 
    timer2.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer2, NULL);

    function2();

    getitimer(ITIMER_VIRTUAL, &timer2);

    printf("Virtual CPU time consumed by function1: %ld microseconds\n", timer1.it_value.tv_usec);
    printf("Virtual CPU time consumed by function2: %ld microseconds\n", timer2.it_value.tv_usec);

    return 0;
}


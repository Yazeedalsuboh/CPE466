#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
 
#define MILLION 10000000

int size = 0;	

pthread_mutex_t mutex; 


void thread_safe_call(void (*f)(int), int param) {
    pthread_mutex_lock(&mutex);
    f(param);
    pthread_mutex_unlock(&mutex); 
};

void increment(int inc) {
	size += inc;
};


void decrement(int dec) {
	size -= dec;
};


void* incThreadRunner(void* ptr) {

	
	for (int i = 0; i < MILLION; i++) {
		//increment(2);
		thread_safe_call(increment, 2);
		//printf("increment: %d\n", size);
	};
	

	return NULL;

};

void* decThreadRunner(void* ptr) {

	
	for (int i = 0; i < MILLION; i++) {
		//decrement(1);
		thread_safe_call(decrement, 1);
		//printf("decrement: %d\n", size);
	};
	

	return NULL;

};

void main() {

	pthread_t incThread, decThread;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_create(&incThread, &attr, incThreadRunner, NULL);
	pthread_create(&decThread, &attr, decThreadRunner, NULL);
	
	
	if (pthread_join(incThread ,NULL) != 0) return;
	if (pthread_join(decThread ,NULL) != 0) return;
	
	
	
	printf("SIZE: %d\n", size);
 };

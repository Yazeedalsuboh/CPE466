#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
 
struct Data{
	 int x;
	 char str[10];
};


void* convert(void* param){
	struct Data* d = (struct Data*) param;
	
	// to absolute value
	d->x = (d->x > 0) ? (d->x) : (-1 * d->x);
	
	// to upper case
	int i;
	for(i = 0;i < sizeof(d->str);i++) {
		d->str[i] = toupper(d->str[i]);
	};
};

void main() {
	struct Data data;

	data.x = -10;
	strcpy(data.str, "Yazeed");

	pthread_t tid; // thread id
	
	// default attributes
	// pointer to the function
	// params
	pthread_create(&tid, NULL, convert, &data);
	
	// waits to finish executing thread
	// store result
	pthread_join(tid, NULL);

	printf("X = %d, STR = %s\n", data.x, data.str);
 };

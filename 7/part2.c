#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

 
struct Data{
	 int x;
	 char str[10];
};


void* convert(void* param){
	struct Data* d = (struct Data*) malloc(sizeof(struct Data*));
	
	struct Data* input = (struct Data*)param;
	
	d->x = input->x > 0 ? input->x : -1*input->x;
	
	int i;
	for(i=0;i<sizeof(d->str);i++) {
		d->str[i] = toupper(input->str[i]);
	};
	
	
	return d;
};
 
 void main(){
	 struct Data data;
	 
	 data.x =-7;
	 strcpy(data.str, "Converting");
	 
	 pthread_t tid;
	 
	 pthread_create(&tid, NULL, convert, &data);
	 
	 struct Data* result;
	 
	 pthread_join(tid, (void**)&result);
	 
	 printf("X = %d, STR = %s\n", data.x, data.str);
	 printf("X = %d, STR = %s\n", result->x, result->str);
 }

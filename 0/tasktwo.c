#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char * argv[]) {

    printf("Arithmetic Operation: %s\n", argv[1]);

    
    if (!strcmp(argv[1], "+")) {
   
    	int result = atoi(argv[2]) + atoi(argv[3]);
    	printf("Addition Result: %i \n", result);
    
    } else if (!strcmp(argv[1], "-")) {
	int result = atoi(argv[2]) - atoi(argv[3]);	
    	
    	printf("Subtraction Result: %i \n", result);
    }else if (!strcmp(argv[1], "x")) {
	int result = atoi(argv[2]) * atoi(argv[3]);
    	
    	printf("Multiplication Result: %i \n", result);
    }else if (!strcmp(argv[1], "/")) {
	int result = atoi(argv[2]) / atoi(argv[3]);
    	
    	printf("Division Result: %i \n", result);
    }; 

    return 0;
}




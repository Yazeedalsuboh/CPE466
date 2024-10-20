#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    printf("Geometric Operation: %s\n", argv[1]);

    // square
    if (!strcmp(argv[1], "s")) {
   
    	int result = atoi(argv[2]) * atoi(argv[2]);
    	printf("Square Result: %i \n", result);
    
        // circle
    } else if (!strcmp(argv[1], "c")) {
	    int result = 3.14 * (atoi(argv[2]) * atoi(argv[2]));	
        printf("Circle Area: %i \n", result);

        // rectangle
    } else if (!strcmp(argv[1], "r")) {

	    int result = atoi(argv[2]) * atoi(argv[3]);
    	printf("Rectangle Area: %i \n", result);
    
    }

    return 0;
}




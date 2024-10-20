#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** tokenizeIntoArray(char* string, char delimiter[]) {

    int maxLength = 0;
    int counter = 0;
   
    char* temp = (char*)malloc(strlen(string) + 1);
    strcpy(temp, string);

    // Tokenization 1: get counter value and maxlengthvalue
    char* portion = strtok(temp, delimiter);
    while (portion != NULL) {
        counter++;
        if (strlen(portion) > maxLength) maxLength = strlen(portion);
        portion = strtok(NULL, delimiter);
    };
    counter++;

    printf("After first tokenization: string: %s, temp: %s\n", string, temp);

    char** arr = (char**)malloc(counter * sizeof(char*));
    int i = 0;

    // Tokenization 2
    portion = strtok(string, delimiter);
    while (portion != NULL && i < counter) {
        
        arr[i] = (char*)malloc((maxLength + 1) * sizeof(char));
        strcpy(arr[i], portion);
        
        
        i++;
        portion = strtok(NULL, delimiter);
    };

    printf("After Second tokenization: string: %s, temp: %s\n", string, temp);



    free(temp);

    return arr;
};



int main(int argc, char** argv) {
    

    // check parameters count
    if (argc != 3) {
        printf("Usage: ./run line delim\n");
        return 0;
    };
    
    // string, delimeter
    char** arr = tokenizeIntoArray(argv[1], argv[2]);
    
   
    // print array
    int i = 0;
    while(arr[i] != NULL){
        printf("[%s]\n", arr[i]);
        i++;
    };
    
}


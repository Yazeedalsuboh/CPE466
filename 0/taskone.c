#include <stdio.h>

int main(int argc, char * argv[]) {
    printf("Number of parameters: %d\n", argc);
    if (argc > 1) {
        printf("Parameters:\n");
        for (int i = 0; i < argc; i++) {
            printf("%d: %s\n", i, argv[i]);
        }
    } else {
            printf("No parameters provided:\n");
    }
    return 0;
}




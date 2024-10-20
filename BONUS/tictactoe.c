#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY ftok("tictactoe.c", 12)
#define TURN_KEY ftok("tictactoe.c", 20)

struct message {
    long mtype;
    int row;
    int col;
};

struct turn_message {
    long mtype;
    int turn;
};

char board[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'} };

void printMatrix(char matrix[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] != 'X' && matrix[i][j] != 'O') {
                printf("_ ");
            } else {
                printf("%c ", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

int checkIfWin(char matrix[3][3]) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (matrix[i][0] == matrix[i][1] && matrix[i][1] == matrix[i][2]) {
            return 1;
        }
        if (matrix[0][i] == matrix[1][i] && matrix[1][i] == matrix[2][i]) {
            return 1;
        }
    }

    // Check diagonals
    if (matrix[0][0] == matrix[1][1] && matrix[1][1] == matrix[2][2]) {
        return 1;
    }
    if (matrix[0][2] == matrix[1][1] && matrix[1][1] == matrix[2][0]) {
        return 1;
    }

    return 0;
};

int gamefinished(char matrix[3][3]) {
    int finished = 1; // Assume the game is finished until proven otherwise

    // Check each cell in the matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // If a cell is not 'X' and not 'O', the game is not finished
            if (matrix[i][j] != 'X' && matrix[i][j] != 'O') {
                finished = 0;
                break; // Exit inner loop
            }
        }
        if (!finished) {
            break; // Exit outer loop
        }
    }

    return finished;
};

int checkValidity(int row, int col) {
    if (row < 0 || col < 0 || row >= 3 || col >= 3) {
        return 0;
    } else if (board[row][col] == 'X' || board[row][col] == 'O') {
        return 0;
    };
    
    return 1;
}


int main(int argc, char *argv[]) {
    pid_t pid1, pid2, pid3;
    

    int msggid = msgget(KEY, IPC_CREAT | 0666);
    int turn_msggid = msgget(TURN_KEY, IPC_CREAT | 0666);

    if ((pid1 = fork()) == 0) {
        // Child 1: Player 1 X
      
        if (argc == 1) {
      
            char *command = "konsole -e ./tictactoe c1";
            system(command);
      
        } else if (strcmp(argv[1], "c1") == 0) {
      
            int row, col;
            struct turn_message turn;
            struct message m;

            system("sleep 2");
            while (1) {
             
             
              
                printf("Player 1 X\n");
           
                //msgrcv(turn_msggid, &turn, sizeof(struct turn_message) - sizeof(long), 3, IPC_NOWAIT);
                //if (turn.turn == 3) exit(0);

                msgrcv(turn_msggid, &turn, sizeof(struct turn_message) - sizeof(long), 1, 0);

                printf("Enter row: ");
                scanf("%d", &row);
                printf("Enter col: ");
                scanf("%d", &col);

                m.mtype = 1;
                m.row = row;
                m.col = col;
           
                msgsnd(msggid, &m, sizeof(struct message) - sizeof(long), 0);
    
                system("clear");
    
            };
    
    
        };
    
    
    } else if ((pid2 = fork()) == 0) {
        // Child 2: Player 2 O
       
        if (argc == 1) {
       
            char *command = "konsole -e ./tictactoe c2";
            system(command);
       
        } else if (strcmp(argv[1], "c2") == 0) {
       
            int row, col;
            struct turn_message turn;
            struct message m;

            system("sleep 2");
       
            while (1) {
        
                
                printf("Player 2 O\n");
                
                //msgrcv(turn_msggid, &turn, sizeof(struct turn_message) - sizeof(long), 3, IPC_NOWAIT);
                //if (turn.turn == 3) exit(0);

                msgrcv(turn_msggid, &turn, sizeof(struct turn_message) - sizeof(long), 2, 0);

                printf("Enter row: ");
                scanf("%d", &row);
                printf("Enter col: ");
                scanf("%d", &col);

                m.mtype = 2;
                m.row = row;
                m.col = col;
                msgsnd(msggid, &m, sizeof(struct message) - sizeof(long), 0);
     
                system("clear");
     
     
     
            };
        };
    } else if ((pid3 = fork()) == 0) {
        // Child 3: Viewboard
        if (argc == 1) {
      
            char *command = "konsole -e ./tictactoe c3";
            system(command);
      
        } else if (strcmp(argv[1], "c3") == 0) {
         
         
            struct turn_message tm;
            struct message p1, p2;

            printMatrix(board);
         
         
            while (1) {
         
         	
                // Player 1 turn
                tm.mtype = 1;
                tm.turn = 1;
                msgsnd(turn_msggid, &tm, sizeof(struct turn_message) - sizeof(long), 0);

                // Wait for valid input
                while (1) {
                    msgrcv(msggid, &p1, sizeof(struct message) - sizeof(long), 1, 0);
                    if (checkValidity(p1.row, p1.col)) break;
                    else {
                    	printf("Player 1 Invalid Inputs\n");
                        msgsnd(turn_msggid, &tm, sizeof(struct turn_message) - sizeof(long), 0);
                    };
                }

		system("clear");
                board[p1.row][p1.col] = 'X';
                printMatrix(board);
		
		
		
		if (gamefinished(board) == 1 && checkIfWin(board) == 0) {
		    printf("Draw\n");
   		
                  

		    system("sleep 2");
		    exit(0);
		} else if (checkIfWin(board) == 1) {
                    printf("Player 1 X Won\n");

   			
                    system("sleep 2");
                    exit(0);
                };

                // Player 2 turn
                tm.mtype = 2;
                tm.turn = 2;
                msgsnd(turn_msggid, &tm, sizeof(struct turn_message) - sizeof(long), 0);

                // Wait for valid input
                while (1) {
                    msgrcv(msggid, &p2, sizeof(struct message) - sizeof(long), 2, 0);
                    if (checkValidity(p2.row, p2.col)) break;
                    else {
                    	printf("Player 2 Invalid Inputs\n");
                        msgsnd(turn_msggid, &tm, sizeof(struct turn_message) - sizeof(long), 0);
                    };
                };

		system("clear");
                board[p2.row][p2.col] = 'O';
                printMatrix(board);
		
	
		if (gamefinished(board) == 1 && checkIfWin(board) == 0) {
			printf("Draw\n");
			

   			
			
			system("sleep 2");
			exit(0);
		} else if (checkIfWin(board) == 1) {
                    printf("Player 2 O Won\n");
                  

   			
                  
                    system("sleep 2");
                    exit(0);
                };
            };
        }
    } else {


        wait(NULL);
        wait(NULL);
        wait(NULL);

        msgctl(msggid, IPC_RMID, NULL);
        msgctl(turn_msggid, IPC_RMID, NULL);
    }

    return 0;
}


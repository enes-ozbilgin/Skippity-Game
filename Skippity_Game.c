//Youtube linkimiz ---> https://youtu.be/LLFjseQIKJg <--- Buradan videoya ulaþabilirsiniz.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int score;
    int A[5];
} Player;

int createboard(char ***board, int *size);
void printboard(char **board, int size);
void showscore(Player *x);
void copyboard(char **board, char ***boardcopy, int size);
char makemove(char ***board, int size, int *x, int *y, char z, Player *score);
int checkmove(char **board, int size, int x, int y, char z);
int moveable(char **board, int size, int x, int y);
void checkscore(Player *score);
int isgameover(char **board, int size);
void savegame(char **board, int size, Player *players, int numPlayers);
void loadgame(char ***board, int *size, Player *players, int numPlayers);
void botmove(char ***board, int size, Player *players);
int simulate_moves(char **board, int size, int x, int y, Player *players, char move);

int main() {
    char **board, y, z = 'y', m, **boardcopy, undo = 'y', save, load, collectedpiece;
    int size, i, j, n, xcord, ycord, flag = 0, player = 1, maxpieces, arr[2];
    Player x[2] = {{0}}; // Initialize the players' scores to zero
    srand(time(0));
    printf("Please choose one:\nSingleplayer (0)\nMultiplayer (1)\n");//we ask the player to pick a game mode.
    scanf("%d", &n);
    getchar();
    while (n != 0 && n != 1) {//if player answers wrongly we ask again.
        printf("\nInvalid input\n");
        printf("Please choose one:\nSingleplayer (0)\nMultiplayer (1)\n");
        scanf("%d", &n);
        getchar();
    }
    printf("\nDo you want to load saved game? (y) (n):");//we ask the player if they want to load previous game.
    scanf(" %c", &load);
    while (load != 'y' && load != 'n') {//if player answers wrongly we ask again.
        printf("\nInvalid input\n");
        printf("\nDo you want to load saved game? (y) (n):");
        scanf(" %c", &load);
        getchar();
    }
    if ( load == 'y' ){
        loadgame(&board,&size,x,2);//if player answers yes we load the previous game.
    } else if ( load == 'n'){
        size = createboard(&board, &size);//we create board if player doesn't want to load previous game.
    }
    boardcopy = (char **)malloc(size * sizeof(char *));//we create board copy.
    for (i = 0; i < size; i++) {
        boardcopy[i] = (char *)malloc(size * sizeof(char));
    }
    while (z != 'n') {//this while loop will keep running until player wants to quit or its game over.
        undo = 'y';flag = 0;
        if (n == 1) {//multiplayer
            copyboard(board, &boardcopy, size);
            while (undo == 'y') {//this loop will run again if player wants to undo their move.
                if (flag == 1) {
                    for (i = 0; i < size; i++) {
                        for (j = 0; j < size; j++) {
                            board[i][j] = boardcopy[i][j];//we undo the move
                        }
                    }
                    if (collectedpiece == 'A') {//we deduct the collected piece from the scoreboard.
                        x[player-1].A[0]--;
                    } else if (collectedpiece == 'B') {
                        x[player-1].A[1]--;
                    } else if (collectedpiece == 'C') {
                        x[player-1].A[2]--;
                    } else if (collectedpiece == 'D') {
                        x[player-1].A[3]--;
                    } else if (collectedpiece == 'E') {
                        x[player-1].A[4]--;
                    }
                }
                printboard(board, size);//show board
                showscore(x);//show score
                printf("\nPlayer %d, it's your turn:\n", player);
                printf("\nPlease enter the (y,x) coordinate of the piece you want to move:");
                scanf("%d %d", &xcord, &ycord);//we get x and y cordinate.
                getchar();
                xcord -= 1;//we decrease by one
                ycord -= 1;//we decrease by one
                while (moveable(board, size, xcord, ycord) != 1) {//if selected piece can not move we ask again
                    printf("\nSelected piece cannot move\n");
                    printboard(board, size);
                    showscore(x);
                    printf("\nPlease enter the (y,x) coordinate of the piece you want to move:");
                    scanf("%d %d", &xcord, &ycord);
                    getchar();
                    xcord -= 1;
                    ycord -= 1;
                }
                printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                scanf(" %c", &m);//we ask which way the player wants to move the piece
                getchar();
                while (checkmove(board, size, xcord, ycord, m) != 1) {//if piece can not move in the choosen direction we ask again.
                    printf("\nSelected piece cannot move in the chosen direction\n");
                    printboard(board, size);
                    showscore(x);
                    printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                    scanf(" %c", &m);
                    getchar();
                }
                if ( player == 1 ) {//we check if its the first player's move.
                    collectedpiece = makemove(&board, size, &xcord, &ycord, m, x);
                    checkscore(x);
                } else if ( player == 2 ) {//we check if its the second player's move.
                    collectedpiece = makemove(&board, size, &xcord, &ycord, m, &x[1]);
                    checkscore(&x[1]);
                }
                printboard(board, size);
                showscore(x);
                if (flag == 0) {//this variable makes sure we can only undo our move one time.
                    printf("\nPlayer %d, do you want to undo your move? (y/n)", player);//we ask if player wants to undo his move.
                    scanf(" %c", &undo);
                    getchar();
                    while (undo != 'y' && undo != 'n') {//if player answers wrongly we ask again.
                        printf("\nInvalid input\n");
                        printf("\nPlayer %d, do you want to undo your move? (y/n)", player);
                        scanf(" %c", &undo);
                        getchar();
                    }
                } else if (flag == 1) {//if player already did undo once they won't be able to undo again.
                    undo = 'n';
                }
                flag = 1;
            } // 1st turn
            while (moveable(board, size, xcord, ycord) == 1) {//this while will keep looping if our moved piece can move again.
                copyboard(board, &boardcopy, size);//we copy the board again
            	flag = 0;
            	undo = 'y';
				while (undo == 'y') {
					if (flag == 1) {
                    	for (i = 0; i < size; i++) {
                        	for (j = 0; j < size; j++) {
                            	board[i][j] = boardcopy[i][j];
                        	}
                    	}
                        if (collectedpiece == 'A') {//we deduct the collected piece if player does an undo.
                            x[player-1].A[0]--;
                        } else if (collectedpiece == 'B') {
                            x[player-1].A[1]--;
                        } else if (collectedpiece == 'C') {
                            x[player-1].A[2]--;
                        } else if (collectedpiece == 'D') {
                            x[player-1].A[3]--;
                        } else if (collectedpiece == 'E') {
                            x[player-1].A[4]--;
                        }
                	}
					printboard(board, size);
                	showscore(x);
                    printf("\nSelected piece can move again\n");
                	printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                	scanf(" %c", &m);
                	getchar();
                	while (checkmove(board, size, xcord, ycord, m) != 1) {
                    	printf("\nSelected piece cannot move in the chosen direction\n");
                    	printboard(board, size);
                    	showscore(x);
                    	printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                    	scanf(" %c", &m);
                    	getchar();
                	}
					if ( player == 1 ) {
                        collectedpiece = makemove(&board, size, &xcord, &ycord, m, x);
                        checkscore(x);
                    } else if ( player == 2 ) {
                        collectedpiece = makemove(&board, size, &xcord, &ycord, m, &x[1]);
                        checkscore(&x[1]);
                    }
					printboard(board, size);
                	showscore(x);
					if (flag == 0) {
                		printf("\nPlayer %d, do you want to undo your move? (y/n)", player);
                		scanf(" %c", &undo);
                		getchar();
                		while (undo != 'y' && undo != 'n') {
                    		printf("\nInvalid input\n");
                    		printf("\nPlayer %d, do you want to undo your move? (y/n)", player);
                    		scanf(" %c", &undo);
                    		getchar();
                		}
            		} else if (flag == 1) {
                		undo = 'n';
            		}
            		flag = 1;
				}
            }//its basically the same thing in 1st turn except its in a loop.
        } else if (n == 0) {//singleplayer
            copyboard(board, &boardcopy, size);
            while (undo == 'y') {
                if (flag == 1) {
                    for (i = 0; i < size; i++) {
                        for (j = 0; j < size; j++) {
                            board[i][j] = boardcopy[i][j];
                        }
                    }
                    if (collectedpiece == 'A') {//we deduct the collected piece if player does an undo.
                        x[0].A[0]--;
                    } else if (collectedpiece == 'B') {
                        x[0].A[1]--;
                    } else if (collectedpiece == 'C') {
                        x[0].A[2]--;
                    } else if (collectedpiece == 'D') {
                        x[0].A[3]--;
                    } else if (collectedpiece == 'E') {
                        x[0].A[4]--;
                    }
                }//its the same as multiplayer except we add an extra function in the end.
                printboard(board, size);
                showscore(x);
                printf("\nPlayer 1, it's your turn:\n");
                printf("\nPlease enter the (y,x) coordinate of the piece you want to move:");
                scanf("%d %d", &xcord, &ycord);
                getchar();
                xcord -= 1;
                ycord -= 1;
                while (moveable(board, size, xcord, ycord) != 1) {
                    printf("\nSelected piece cannot move\n");
                    printboard(board, size);
                    showscore(x);
                    printf("\nPlease enter the (y,x) coordinate of the piece you want to move:");
                    scanf("%d %d", &xcord, &ycord);
                    getchar();
                    xcord -= 1;
                    ycord -= 1;
                }
                printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                scanf(" %c", &m);
                getchar();
                while (checkmove(board, size, xcord, ycord, m) != 1) {
                    printf("\nSelected piece cannot move in the chosen direction\n");
                    printboard(board, size);
                    showscore(x);
                    printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                    scanf(" %c", &m);
                    getchar();
                }
                collectedpiece = makemove(&board, size, &xcord, &ycord, m, x);
                checkscore(x);
                printboard(board, size);
                showscore(x);
                if (flag == 0) {
                    printf("\nPlayer 1, do you want to undo your move? (y/n)");
                    scanf(" %c", &undo);
                    getchar();
                    while (undo != 'y' && undo != 'n') {
                        printf("\nInvalid input\n");
                        printf("\nPlayer 1, do you want to undo your move? (y/n)");
                        scanf(" %c", &undo);
                        getchar();
                    }
                } else if (flag == 1) {
                    undo = 'n';
                }
                flag = 1;
            } // 1st turn
            while (moveable(board, size, xcord, ycord) == 1) {
                copyboard(board, &boardcopy, size);
            	flag = 0;
            	undo = 'y';
				while (undo == 'y') {
					if (flag == 1) {
                    	for (i = 0; i < size; i++) {
                        	for (j = 0; j < size; j++) {
                            	board[i][j] = boardcopy[i][j];
                        	}
                    	}
                        if (collectedpiece == 'A') {//we deduct the collected piece if player does an undo.
                            x[0].A[0]--;
                        } else if (collectedpiece == 'B') {
                            x[0].A[1]--;
                        } else if (collectedpiece == 'C') {
                            x[0].A[2]--;
                        } else if (collectedpiece == 'D') {
                            x[0].A[3]--;
                        } else if (collectedpiece == 'E') {
                            x[0].A[4]--;
                        }
                	}
					printboard(board, size);
                	showscore(x);
                    printf("\nSelected piece can move again\n");
                	printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                	scanf(" %c", &m);
                	getchar();
                	while (checkmove(board, size, xcord, ycord, m) != 1) {
                    	printf("\nSelected piece cannot move in the chosen direction\n");
                    	printboard(board, size);
                    	showscore(x);
                    	printf("\nPlease enter which way you want the piece to move:(w)(a)(s)(d)\n");
                    	scanf(" %c", &m);
                    	getchar();
                	}
                    collectedpiece = makemove(&board, size, &xcord, &ycord, m, x);
                    checkscore(x);
					printboard(board, size);
                	showscore(x);
					if (flag == 0) {
                		printf("\nPlayer 1, do you want to undo your move? (y/n)");
                		scanf(" %c", &undo);
                		getchar();
                		while (undo != 'y' && undo != 'n') {
                    		printf("\nInvalid input\n");
                    		printf("\nPlayer 1, do you want to undo your move? (y/n)");
                    		scanf(" %c", &undo);
                    		getchar();
                		}
            		} else if (flag == 1) {
                		undo = 'n';
            		}
            		flag = 1;
				}
            }
            botmove(&board,size,&x[1]);//this the only difference between singleplayer and multiplayer.
        }
        if ( player == 2 ){
            printf("\nDo you want to continue playing? (y/n): ");//we ask the player if he wants to continue playing.
            scanf(" %c", &z);
            getchar();
            while (z != 'y' && z != 'n') {//if player answers wrongly we ask again.
                printf("\nInvalid input\n");
                printf("\nDo you want to continue playing? (y/n): ");
                scanf(" %c", &z);
                getchar();
            }
        }
        if ( player == 1 ){//we update the player so we can track whose turn it is.
            player++;
        } else if ( player == 2 ){//we update the player so we can track whose turn it is.
            player--;
        }
        if (z == 'n') {//if player wants to quit we ask if they want to save the current game.
            printf("\nDo you want to save this game? (y) (n)\n");
            scanf(" %c", &save);
            getchar();
            while (save != 'y' && save != 'n') {//if player answers wrongly we ask again.
            printf("\nInvalid input\n");
            printf("\nDo you want to save this game? (y) (n)\n");
            scanf(" %c", &save);
            getchar();
            }
            if (save == 'y'){//if player answers yes we save the game.
                savegame(board,size,x,2);
            }
        }
        if ( isgameover(board,size) == 1 ){// we check if its game over.
            printf("\n\nGAMEOVER\n\n");//its gameover.
            if (x[0].score > x[1].score){//we look at these conditions to see who won.
                printf("\n\nCongrats Player 1 you have won the game\n\n");
            } else if (x[0].score < x[1].score){
                printf("\n\nCongrats Player 2 you have won the game\n\n");
            } else if (x[0].score == x[1].score){
                printf("\n\nINCREDIBLE its a DRAW\n\n");
            }
            z = 'n';//with this we quit the game.
        }
    }
    for (i = 0; i < size; i++) {
        free(board[i]);//free the board for extra points :)
    }
    free(board);
    return 0;
}
//The main function is where we do most of the games logic.
int createboard(char ***board, int *size) {
    int i, j;
    printf("Enter the size of the board (max: 20): ");
    scanf("%d", size);
    getchar();

    while (*size > 20 || *size < 4) {
        printf("Invalid size. Maximum size is 20.\n");
        printf("Enter the size of the board (max: 20): ");
        scanf("%d", size);
    }

    *board = (char **)malloc(*size * sizeof(char *));
    for (i = 0; i < *size; i++) {
        (*board)[i] = (char *)malloc(*size * sizeof(char));
    }

    char values[5] = {'A', 'B', 'C', 'D', 'E'};
    for (i = 0; i < *size; i++) {
        for (j = 0; j < *size; j++) {
            int index = rand() % 5;
            (*board)[i][j] = values[index];
        }
    }
    (*board)[(*size / 2) - 1][(*size / 2) - 1] = '_';
    (*board)[(*size / 2) - 1][(*size / 2)] = '_';
    (*board)[(*size / 2)][(*size / 2) - 1] = '_';
    (*board)[(*size / 2)][(*size / 2)] = '_';
    return *size;
}
//The createboard function initializes the game board with random initial positions of pieces.
void printboard(char **board, int size) {
    int i, j;
    char x[11] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    printf("\n");
    for (i = 0; i < size; i++) {
        if (i < 9) {
            printf("%d ", i + 1);
        } else {
            printf("%c ", x[i - 9]);
        }
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'A') {
                printf("\033[31m%c \033[0m", board[i][j]);
            } else if (board[i][j] == 'B') {
                printf("\033[32m%c \033[0m", board[i][j]);
            } else if (board[i][j] == 'C') {
                printf("\033[33m%c \033[0m", board[i][j]);
            } else if (board[i][j] == 'D') {
                printf("\033[34m%c \033[0m", board[i][j]);
            } else if (board[i][j] == 'E') {
                printf("\033[35m%c \033[0m", board[i][j]);
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
    printf("  ");
    for (i = 0; i < size; i++) {
        if (i < 9) {
            printf("%d ", i + 1);
        } else
            printf("%c ", x[i - 9]);
    }
    printf("\n");
}
//The printboard function displays the current state of the game board.
void showscore(Player *x) {
    printf("\n Player 1 : Enes");
    printf("\n\033[36m Score \033[0m");
    printf("| ");
    printf("\033[31mA \033[0m");
    printf("| ");
    printf("\033[32mB \033[0m");
    printf("| ");
    printf("\033[33mC \033[0m");
    printf("| ");
    printf("\033[34mD \033[0m");
    printf("| ");
    printf("\033[35mE \033[0m");
    printf("|\n");
    printf(" -----------------------------\n");
    printf("   %d   | %d | %d | %d | %d | %d |\n", x[0].score, x[0].A[0], x[0].A[1], x[0].A[2], x[0].A[3], x[0].A[4]);
    printf("\n Player 2 : Betul");
    printf("\n\033[36m Score \033[0m");
    printf("| ");
    printf("\033[31mA \033[0m");
    printf("| ");
    printf("\033[32mB \033[0m");
    printf("| ");
    printf("\033[33mC \033[0m");
    printf("| ");
    printf("\033[34mD \033[0m");
    printf("| ");
    printf("\033[35mE \033[0m");
    printf("|\n");
    printf(" -----------------------------\n");
    printf("   %d   | %d | %d | %d | %d | %d |\n", x[1].score, x[1].A[0], x[1].A[1], x[1].A[2], x[1].A[3], x[1].A[4]);
}
//The showscore function prints the scores of the two players and the distribution of their collected pieces.
void copyboard(char **board, char ***boardcopy, int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            (*boardcopy)[i][j] = board[i][j];
        }
    }
}
//The copyboard function copies the currents state of the board on boardcopy variable.
int moveable(char **board, int size, int x, int y) {
    if (x >= size || y >= size || x < 0 || y < 0) {
        return 0;
    }
    if (x + 2 < size && board[x][y] != '_' && board[x + 2][y] == '_' && board[x + 1][y] != '_') {
        return 1;
    } 
    if (x - 2 >= 0 && board[x][y] != '_' && board[x - 2][y] == '_' && board[x - 1][y] != '_') {
        return 1;
    } 
    if (y + 2 < size && board[x][y] != '_' && board[x][y + 2] == '_' && board[x][y + 1] != '_') {
        return 1;
    } 
    if (y - 2 >= 0 && board[x][y] != '_' && board[x][y - 2] == '_' && board[x][y - 1] != '_') {
        return 1;
    }
    return 0;
}
//The moveable function checks if the piece can move in any 4 directions.
char makemove(char ***board, int size, int *x, int *y, char z, Player *score) {
    char temp;
    char collectedpiece;
    if (z == 'w') {
        (*board)[*x - 2][*y] = (*board)[*x][*y];
        (*board)[*x][*y] = '_';
        if ((*board)[*x - 1][*y] == 'A') {
            score->A[0]++;
            collectedpiece = 'A';
        } else if ((*board)[*x - 1][*y] == 'B') {
            score->A[1]++;
            collectedpiece = 'B';
        } else if ((*board)[*x - 1][*y] == 'C') {
            score->A[2]++;
            collectedpiece = 'C';
        } else if ((*board)[*x - 1][*y] == 'D') {
            score->A[3]++;
            collectedpiece = 'D';
        } else if ((*board)[*x - 1][*y] == 'E') {
            score->A[4]++;
            collectedpiece = 'E';
        }
        (*board)[*x - 1][*y] = '_';
        *x -= 2;
    } else if (z == 's') {
        (*board)[*x + 2][*y] = (*board)[*x][*y];
        (*board)[*x][*y] = '_';
        if ((*board)[*x + 1][*y] == 'A') {
            score->A[0]++;
            collectedpiece = 'A';
        } else if ((*board)[*x + 1][*y] == 'B') {
            score->A[1]++;
            collectedpiece = 'B';
        } else if ((*board)[*x + 1][*y] == 'C') {
            score->A[2]++;
            collectedpiece = 'C';
        } else if ((*board)[*x + 1][*y] == 'D') {
            score->A[3]++;
            collectedpiece = 'D';
        } else if ((*board)[*x + 1][*y] == 'E') {
            score->A[4]++;
            collectedpiece = 'E';
        }
        (*board)[*x + 1][*y] = '_';
        *x += 2;
    } else if (z == 'a') {
        (*board)[*x][*y - 2] = (*board)[*x][*y];
        (*board)[*x][*y] = '_';
        if ((*board)[*x][*y - 1] == 'A') {
            score->A[0]++;
            collectedpiece = 'A';
        } else if ((*board)[*x][*y - 1] == 'B') {
            score->A[1]++;
            collectedpiece = 'B';
        } else if ((*board)[*x][*y - 1] == 'C') {
            score->A[2]++;
            collectedpiece = 'C';
        } else if ((*board)[*x][*y - 1] == 'D') {
            score->A[3]++;
            collectedpiece = 'D';
        } else if ((*board)[*x][*y - 1] == 'E') {
            score->A[4]++;
            collectedpiece = 'E';
        }
        (*board)[*x][*y - 1] = '_';
        *y -= 2;
    } else if (z == 'd') {
        (*board)[*x][*y + 2] = (*board)[*x][*y];
        (*board)[*x][*y] = '_';
        if ((*board)[*x][*y + 1] == 'A') {
            score->A[0]++;
            collectedpiece = 'A';
        } else if ((*board)[*x][*y + 1] == 'B') {
            score->A[1]++;
            collectedpiece = 'B';
        } else if ((*board)[*x][*y + 1] == 'C') {
            score->A[2]++;
            collectedpiece = 'C';
        } else if ((*board)[*x][*y + 1] == 'D') {
            score->A[3]++;
            collectedpiece = 'D';
        } else if ((*board)[*x][*y + 1] == 'E') {
            score->A[4]++;
            collectedpiece = 'E';
        }
        (*board)[*x][*y + 1] = '_';
        *y += 2;
    }
    return collectedpiece;
}
//The makemove function is responsible for moving a piece on the board based on user input.
//It also updates the collected pieces accordingly.
int checkmove(char **board, int size, int x, int y, char z) {
    if (z == 'w') {
        if (x - 2 >= 0 && board[x - 2][y] == '_' && board[x - 1][y] != '_') {
            return 1;
        } else {
            return 0;
        }
    } else if (z == 's') {
        if (x + 2 < size && board[x + 2][y] == '_' && board[x + 1][y] != '_') {
            return 1;
        } else {
            return 0;
        }
    } else if (z == 'a') {
        if (y - 2 >= 0 && board[x][y - 2] == '_' && board[x][y - 1] != '_') {
            return 1;
        } else {
            return 0;
        }
    } else if (z == 'd') {
        if (y + 2 < size && board[x][y + 2] == '_' && board[x][y + 1] != '_') {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}
//The checkmove function verifies if a move is valid for a given piece on the board.
void checkscore(Player *score){
    int A, B, C, D, E, min;
    A = score->A[0];
    B = score->A[1];
    C = score->A[2];
    D = score->A[3];
    E = score->A[4];
    min = A; 
    if (B < min) {
        min = B;
    } if (C < min) {
        min = C;
    } if (D < min) {
        min = D;
    } if (E < min) {
        min = E;
    }
    score->score = min;
}
//The checkscore function updates the scores accordingly.
int isgameover(char **board, int size){
    int i, j;
    for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            if( moveable(board,size,i,j) == 1){
                return 0;
            }
        }
    }
    return 1;
}
//The isgameover function determines if the game is over by checking if any pieces are movable.
void savegame(char **board, int size, Player *players, int numPlayers) {
	int i, j;
    FILE *file = fopen("savedgame.txt", "w");
    if (!file) {
        printf("Error saving game.\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fprintf(file, "%c ", board[i][j]);
        }
        fprintf(file, "\n");
    }

    for (i = 0; i < 2; i++) {
        fprintf(file, "%d ", players[i].score);
        for (j = 0; j < 5; j++) {
            fprintf(file, "%d ", players[i].A[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("\nGame saved successfully.\n");
}
//The savegame and loadgame functions allow the user to save and load the game state to/from a file named "savedgame.txt".
void loadgame(char ***board, int *size, Player *players, int numPlayers) {
    int i, j;
    FILE *file = fopen("savedgame.txt", "r");
    if (!file) {
        printf("No saved game found.\n");
        return;
    }

    fscanf(file, "%d", size);// Read board size
    getchar(); // consume newline
    
    *board = (char **)malloc(*size * sizeof(char *));// Allocate memory for the board
    for (i = 0; i < *size; i++) {
        (*board)[i] = (char *)malloc(*size * sizeof(char));
    }
    
    for (i = 0; i < *size; i++) {// Read board state
        for (j = 0; j < *size; j++) {
            fscanf(file, " %c ", &(*board)[i][j]); // Note the extra space before %c to consume leading whitespace
        }
    }
    
    for (i = 0; i < numPlayers; i++) {// Read players' scores
        fscanf(file, "%d", &players[i].score);
        for (j = 0; j < 5; j++) {
            fscanf(file, "%d", &players[i].A[j]);
        }
    }
    fclose(file);
    printf("\nGame loaded successfully.\n");
}
void botmove(char ***board, int size, Player *players) {
    int i, j, k, m, n;
    int best_collected = -1;
    char best_move, move, trash;
    int best_x = -1, best_y = -1, collected;
    char moves[4] = {'w', 'a', 's', 'd'};
    char **boardcopy = (char **)malloc(size * sizeof(char *));
    for (i = 0; i < size; i++) {
        boardcopy[i] = (char *)malloc(size * sizeof(char));
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (moveable((*board), size, i, j) && (*board)[i][j] != '_') {
                for (k = 0; k < 4; k++) {
                    move = moves[k];
                    if (checkmove((*board), size, i, j, move)) {
                        // Copy the board state
                        for (m = 0; m < size; m++) {
                            for (n = 0; n < size; n++) {
                                boardcopy[m][n] = (*board)[m][n];
                            }
                        }
                        Player temp_player = *players; // Create a temporary player

                        // Simulate the moves
                        collected = simulate_moves(boardcopy, size, i, j, &temp_player, move);
                        
                        // Update the best move
                        if (collected > best_collected) {
                            best_collected = collected;
                            best_x = i;
                            best_y = j;
                            best_move = move;
                        }
                    }
                }
            }
        }
    }

    trash = makemove(board, size, &best_x, &best_y, best_move, players);
    checkscore(players);
    while (moveable((*board), size, best_x, best_y) == 1) {
        if (checkmove((*board), size, best_x, best_y, 'w') == 1) {
            trash = makemove(board, size, &best_x, &best_y, 'w', players);
        } else if (checkmove((*board), size, best_x, best_y, 's') == 1) {
            trash = makemove(board, size, &best_x, &best_y, 's', players);
        } else if (checkmove((*board), size, best_x, best_y, 'a') == 1) {
            trash = makemove(board, size, &best_x, &best_y, 'a', players);
        } else if (checkmove((*board), size, best_x, best_y, 'd') == 1) {
            trash = makemove(board, size, &best_x, &best_y, 'd', players);
        }
        checkscore(players);
    }

    // Free allocated memory for boardcopy
    for (i = 0; i < size; i++) {
        free(boardcopy[i]);//extra points :)
    }
    free(boardcopy);
    return;
}
//The botmove function implements the bot's move logic in single-player mode.
//It tries to find the best move based on the maximum number of pieces collected in the subsequent moves.
int simulate_moves(char **board, int size, int x, int y, Player *players, char move) {
    int collected = 0;
    int max_collected = 0;
    char original = board[x][y], trash;
    
    // Make the initial move if within bounds
    if (moveable(board, size, x, y)) {
        trash = makemove(&board, size, &x, &y, move, players);
        collected++;
    }
    
    // Explore all possible moves from the new position
    if (moveable(board, size, x, y)) {
        if (checkmove(board, size, x, y, 'w')) {
            int temp_collected = simulate_moves(board, size, x, y, players, 'w');
            if (temp_collected > max_collected) {
                max_collected = temp_collected;
            }
        }
        if (checkmove(board, size, x, y, 's')) {
            int temp_collected = simulate_moves(board, size, x, y, players, 's');
            if (temp_collected > max_collected) {
                max_collected = temp_collected;
            }
        }
        if (checkmove(board, size, x, y, 'a')) {
            int temp_collected = simulate_moves(board, size, x, y, players, 'a');
            if (temp_collected > max_collected) {
                max_collected = temp_collected;
            }
        }
        if (checkmove(board, size, x, y, 'd')) {
            int temp_collected = simulate_moves(board, size, x, y, players, 'd');
            if (temp_collected > max_collected) {
                max_collected = temp_collected;
            }
        }
    }
    
    board[x][y] = original;  // Restore the original piece
    return collected + max_collected; // Return the total collected pieces
}
//simulate_moves function evaluates the potential outcome of a move by simulating it on a copy of the game board.
//This allows the botmove function to assess the desirability of a move before committing to it.

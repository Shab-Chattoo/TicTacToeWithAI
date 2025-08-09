#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// Initial board setup
char square[10] = {'o','1','2','3','4','5','6','7','8','9'};

// Function declarations
int checkWin();
void drawBoard();
int evaluate(char b[10]);               // Dual heuristic
int evaluateWinLoss(char b[10]);        // Heuristic 1
int evaluatePosition(char b[10]);       // Heuristic 2
int minimax(char board[10], int depth, int isMax, int alpha, int beta);
int findBestMove();

int main(){
    system("color 4a");
    int player = 1, i ,choice;
    char mark;

    do {
        drawBoard();
        player = (player % 2) ? 1 : 2;

        if(player == 1) {
            printf("Player %d (X), enter the choice : ",player);
            scanf("%d",&choice);
            mark = 'X';
            if(choice >= 1 && choice <= 9 && square[choice] == choice + '0')
                square[choice] = mark;
            else {
                printf("Invalid option !");
                player--;
                getch();
            }
        } else {
            printf("AI is making its move...\n");
            int bestMove = findBestMove();
            square[bestMove] = 'O';
        }

        i = checkWin();
        player++;

    } while(i == -1);

    drawBoard();
    if(i==1){
        printf("==> Player %d won",--player);
    }
    else {
        printf("==> Game draw");
    }
    getch();
    return 0;
}

// Rule-based game outcome checker
int checkWin(){
    if(square[1] == square[2] && square[2] == square[3]) return 1;
    if(square[4] == square[5] && square[5] == square[6]) return 1;
    if(square[7] == square[8] && square[8] == square[9]) return 1;
    if(square[1] == square[4] && square[4] == square[7]) return 1;
    if(square[2] == square[5] && square[5] == square[8]) return 1;
    if(square[3] == square[6] && square[6] == square[9]) return 1;
    if(square[1] == square[5] && square[5] == square[9]) return 1;
    if(square[3] == square[5] && square[5] == square[7]) return 1;

    if(square[1] != '1' && square[2] != '2' && square[3] != '3' &&
       square[4] != '4' && square[5] != '5' && square[6] != '6' &&
       square[7] != '7' && square[8] != '8' && square[9] != '9')
        return 0;

    return -1;
}

// Display the board
void drawBoard(){
    system("cls");
    printf("\n\n\t Tic Tac Toe \n\n");
    printf("Player1 (X) - AI (O) \n\n\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",square[1],square[2],square[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",square[4],square[5],square[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n",square[7],square[8],square[9]);
    printf("     |     |     \n");
}

// Dual heuristic: combines win/loss and positional scoring
int evaluate(char b[10]) {
    return evaluateWinLoss(b) + evaluatePosition(b);
}

// Heuristic 1: Basic win/loss outcome
int evaluateWinLoss(char b[10]) {
    if (b[1] == b[2] && b[2] == b[3]) return (b[1] == 'O') ? 10 : (b[1] == 'X') ? -10 : 0;
    if (b[4] == b[5] && b[5] == b[6]) return (b[4] == 'O') ? 10 : (b[4] == 'X') ? -10 : 0;
    if (b[7] == b[8] && b[8] == b[9]) return (b[7] == 'O') ? 10 : (b[7] == 'X') ? -10 : 0;
    if (b[1] == b[4] && b[4] == b[7]) return (b[1] == 'O') ? 10 : (b[1] == 'X') ? -10 : 0;
    if (b[2] == b[5] && b[5] == b[8]) return (b[2] == 'O') ? 10 : (b[2] == 'X') ? -10 : 0;
    if (b[3] == b[6] && b[6] == b[9]) return (b[3] == 'O') ? 10 : (b[3] == 'X') ? -10 : 0;
    if (b[1] == b[5] && b[5] == b[9]) return (b[1] == 'O') ? 10 : (b[1] == 'X') ? -10 : 0;
    if (b[3] == b[5] && b[5] == b[7]) return (b[3] == 'O') ? 10 : (b[3] == 'X') ? -10 : 0;
    return 0;
}

// Heuristic 2: Positional advantages
int evaluatePosition(char b[10]) {
    int score = 0;

    // Center
    if (b[5] == 'O') score += 3;
    else if (b[5] == 'X') score -= 3;

    // Corners
    int corners[4] = {1, 3, 7, 9};
    for (int i = 0; i < 4; i++) {
        if (b[corners[i]] == 'O') score += 2;
        else if (b[corners[i]] == 'X') score -= 2;
    }

    // Edges
    int edges[4] = {2, 4, 6, 8};
    for (int i = 0; i < 4; i++) {
        if (b[edges[i]] == 'O') score += 1;
        else if (b[edges[i]] == 'X') score -= 1;
    }

    return score;
}

// Minimax algorithm with Alpha-Beta pruning
int minimax(char b[10], int depth, int isMax, int alpha, int beta) {
    int score = evaluate(b);
    if(score == 10 || score == -10) return score;
    
    int movesLeft = 0;
    for(int i = 1; i <= 9; i++) {
        if(b[i] == i + '0') {
            movesLeft = 1;
            break;
        }
    }
    if(!movesLeft) return 0;

    if(isMax) {
        int best = -1000;
        for(int i = 1; i <= 9; i++) {
            if(b[i] == i + '0') {
                b[i] = 'O';
                int val = minimax(b, depth + 1, 0, alpha, beta);
                best = (val > best) ? val : best;
                alpha = (alpha > best) ? alpha : best;
                b[i] = i + '0';
                if(beta <= alpha) break;
            }
        }
        return best;
    } else {
        int best = 1000;
        for(int i = 1; i <= 9; i++) {
            if(b[i] == i + '0') {
                b[i] = 'X';
                int val = minimax(b, depth + 1, 1, alpha, beta);
                best = (val < best) ? val : best;
                beta = (beta < best) ? beta : best;
                b[i] = i + '0';
                if(beta <= alpha) break;
            }
        }
        return best;
    }
}

// AI picks the best move using dual heuristic + pruning
int findBestMove() {
    int bestVal = -1000;
    int move = -1;

    for(int i = 1; i <= 9; i++) {
        if(square[i] == i + '0') {
            char temp = square[i];
            square[i] = 'O';
            int moveVal = minimax(square, 0, 0, -1000, 1000);
            square[i] = temp;
            if(moveVal > bestVal) {
                move = i;
                bestVal = moveVal;
            }
        }
    }
    return move;
}


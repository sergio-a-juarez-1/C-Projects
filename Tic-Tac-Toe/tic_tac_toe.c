/* Author: Sergio Juarez
Purpose:  An enhanced Tic Tac Toe game with an AI, Score Tracking, 
          Fixed Input Buffering, and alternating first turns across rematches.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Game States & Mode Settings
#define MODE_TWO_PLAYER 1
#define MODE_VS_COMPUTER 2
#define DIFF_EASY 1
#define DIFF_MEDIUM 2
#define DIFF_HARD 3

char square[10]; 
int choice = 0, player = 0;
int game_mode = 0, difficulty = 0;

// Score Counter Structure
struct Scoreboard {
    int player1_wins;
    int player2_wins; 
    int draws;
} score = {0, 0, 0};

int checkForWin();
void displayBoard();
int markBoard(char mark);
int get_computer_move();
int find_win_or_block(char player_mark);
int get_random_move();
void resetBoard();
int minimax(int depth, int is_max);

int main()
{
    srand((unsigned int)time(NULL));
    char play_again = 'n';
    
    // TRACKING WHO STARTS: Match 1 = Player 1 starts. Match 2 = Player 2 starts.
    int match_starter = 1; 

    // 1. Select Game Mode
    while (game_mode != MODE_TWO_PLAYER && game_mode != MODE_VS_COMPUTER) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printf("--- TIC-TAC-TOE ---\n");
        printf("1. Two Players\n");
        printf("2. Against the Computer\n");
        printf("Select Game Mode (1-2): ");
        if (scanf("%d", &game_mode) != 1) game_mode = 0;
        while (getchar() != '\n'); 
    }

    // 2. Select Difficulty
    if (game_mode == MODE_VS_COMPUTER) {
        while (difficulty < DIFF_EASY || difficulty > DIFF_HARD) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
            printf("--- SELECT DIFFICULTY ---\n");
            printf("1. Easy\n");
            printf("2. Medium\n");
            printf("3. Hard\n");
            printf("Select Difficulty (1-3): ");
            if (scanf("%d", &difficulty) != 1) difficulty = 0;
            while (getchar() != '\n'); 
        }
    }

    // Outer Rematch Loop
    do {
        resetBoard();
        int gameStatus = -1;
        char mark = ' ';
        
        // Assign the starting player based on our match counter
        player = match_starter; 

        do
        {
            displayBoard();
            
            // Player 1 is ALWAYS 'X' and Player 2/Computer is ALWAYS 'O'
            mark = (player == 1) ? 'X' : 'O';

            // Decide who is picking the square
            if (game_mode == MODE_VS_COMPUTER && player == 2) {
                // Computer thinking phase
                printf("Computer (%c) is thinking...\n", mark);
                
                // Add a small artificial delay so the computer turn feels natural
                #ifdef _WIN32
                    _sleep(600); // 0.6 seconds on Windows
                #else
                    struct timespec ts = {0, 600000000}; 
                    nanosleep(&ts, NULL); // 0.6 seconds on POSIX/Linux
                #endif
                
                choice = get_computer_move();
            } else {
                printf("Player %d (%c), enter a number: ", player, mark);
                if (scanf("%d", &choice) != 1) {
                    choice = 0; 
                }
                while (getchar() != '\n'); // flush out entry buffer (protects against typing '11')
            }

            // Try to place the piece. If invalid, retry without switching turns
            if (markBoard(mark) == 0) {
                continue; 
            }

            gameStatus = checkForWin();
            
            if (gameStatus == -1) {
                player = (player == 1) ? 2 : 1;
            }

        } while (gameStatus == -1);

        displayBoard();

        // Update Scores and Print Results
        if (gameStatus == 1) {
            if (game_mode == MODE_VS_COMPUTER && player == 2) {
                printf("==>\aThe Computer wins!\n");
                score.player2_wins++;
            } else {
                printf("==>\aPlayer %d wins!\n", player);
                if (player == 1) score.player1_wins++;
                else score.player2_wins++;
            }
        } else {
            printf("==>\aGame draw\n");
            score.draws++;
        }

        // ALTERNATE WHO STARTS NEXT MATCH:
        // If current match starter was 1, next time it becomes 2. If it was 2, it becomes 1.
        match_starter = (match_starter == 1) ? 2 : 1;

        // Rematch prompt
        printf("\nWould you like to play again? (y/n): ");
        scanf("%c", &play_again);
        while (getchar() != '\n'); 

    } while (play_again == 'y' || play_again == 'Y');

    printf("\nFinal Score:\n");
    if (game_mode == MODE_VS_COMPUTER) {
        printf("Player (X): %d  |  Computer (O): %d  |  Draws: %d\n", score.player1_wins, score.player2_wins, score.draws);
    } else {
        printf("Player 1 (X): %d  |  Player 2 (O): %d  |  Draws: %d\n", score.player1_wins, score.player2_wins, score.draws);
    }
    printf("\nThanks for playing!\n");
    return 0;
}

void resetBoard()
{
    square[0] = 'o';
    for (int i = 1; i <= 9; i++) {
        square[i] = '0' + i;
    }
}

int checkForWin()
{
    if (square[1] == square[2] && square[2] == square[3]) return 1;
    if (square[4] == square[5] && square[5] == square[6]) return 1;
    if (square[7] == square[8] && square[8] == square[9]) return 1;

    if (square[1] == square[4] && square[4] == square[7]) return 1;
    if (square[2] == square[5] && square[5] == square[8]) return 1;
    if (square[3] == square[6] && square[6] == square[9]) return 1;

    if (square[1] == square[5] && square[5] == square[9]) return 1;
    if (square[3] == square[5] && square[5] == square[7]) return 1;

    if (square[1] != '1' && square[2] != '2' && square[3] != '3' &&
        square[4] != '4' && square[5] != '5' && square[6] != '6' && 
        square[7] != '7' && square[8] != '8' && square[9] != '9') {
        return 0;
    }

    return -1; 
}

void displayBoard()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("\n\n\tTic Tac Toe\n\n");
    
    if (game_mode == MODE_VS_COMPUTER) {
        printf("SCORE -> Player (X): %d   Computer (O): %d   Draws: %d\n", score.player1_wins, score.player2_wins, score.draws);
        printf("--------------------------------------------------\n\n");
    } else {
        printf("SCORE -> Player 1 (X): %d   Player 2 (O): %d   Draws: %d\n", score.player1_wins, score.player2_wins, score.draws);
        printf("--------------------------------------------------\n\n");
    }

    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[1], square[2], square[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[4], square[5], square[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[7], square[8], square[9]);
    printf("     |     |     \n\n");
}

int markBoard(char mark)
{
    if (choice >= 1 && choice <= 9 && square[choice] == ('0' + choice)) {
        square[choice] = mark;
        return 1;
    } else {
        if (game_mode == MODE_TWO_PLAYER || player == 1) {
            printf("Invalid move! Press Enter to try again.");
            getchar(); 
        }
        return 0;
    }
}

int get_computer_move()
{
    if (difficulty == DIFF_EASY) {
        return get_random_move();
    }

    if (difficulty == DIFF_MEDIUM && (rand() % 2 == 0)) {
        return get_random_move();
    }

    int best_score = -1000;
    int best_move = 0;

    for (int i = 1; i <= 9; i++) {
        if (square[i] == ('0' + i)) {
            square[i] = 'O'; 
            int move_score = minimax(0, 0); 
            square[i] = '0' + i; 

            if (move_score > best_score) {
                best_score = move_score;
                best_move = i;
            }
        }
    }
    return best_move;
}

int minimax(int depth, int is_max)
{
    int status = checkForWin();
    
    if (status == 1) {
        // Minimax evaluates absolute victory conditions
        // if is_max is true, it means 'O' just simulated a winning path
        return is_max ? -10 + depth : 10 - depth;
    }
    if (status == 0) return 0; 

    if (is_max) { 
        int best_score = -1000;
        for (int i = 1; i <= 9; i++) {
            if (square[i] == ('0' + i)) {
                square[i] = 'O';
                int score_val = minimax(depth + 1, 0);
                square[i] = '0' + i;
                if (score_val > best_score) best_score = score_val;
            }
        }
        return best_score;
    } else { 
        int best_score = 1000;
        for (int i = 1; i <= 9; i++) {
            if (square[i] == ('0' + i)) {
                square[i] = 'X';
                int score_val = minimax(depth + 1, 1);
                square[i] = '0' + i;
                if (score_val < best_score) best_score = score_val;
            }
        }
        return best_score;
    }
}

int get_random_move()
{
    int available[10];
    int count = 0;
    for (int i = 1; i <= 9; i++) {
        if (square[i] == ('0' + i)) {
            available[count++] = i;
        }
    }
    return (count > 0) ? available[rand() % count] : 0;
}

#include<pthread.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "assign3.h"

pthread_mutex_t mutex;

/// @brief Struct to hold parameters passed to each thread.
typedef struct {
    // index of row / col to check
    int i;
    // chess board to check
    char** board;
} board_args;


/// @brief Dynamically allocates memory for BOARD_WIDTH X BOARD_WIDTH char array.
/// @returns Array of strings (2D char array) of length BOARD_WIDTH initalized to all 0's.
char **create_board()
{
    char **board = (char **)calloc((BOARD_WIDTH + 1), sizeof(char *));
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        board[i] = (char *)calloc((BOARD_WIDTH + 1), sizeof(char));
        memset(board[i], '0', BOARD_WIDTH);
    }
    return board;
}

/// @brief Frees all dynamically allocated memory for a 2D char array.
void destroy_board(char **board)
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        free(board[i]);
    }
    free(board);
}


/// @brief Prints all elements in a 2D char array.
void print_board(char **board)
{

    if (board == NULL)
    {
        printf("\n - Board Empty\n");
        return;
    }

    printf("\n\n - Board: -\n\n");

    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n ----------\n\n");
}


/// @brief Gets row and col for each queen in the board, and sets element of the board in index [row][col] to 1.
/// @returns 1 if user input is in the valid rage of coordinates for the board, 0 otherwise.
int get_queens(char **board)
{
    int row;
    int col;
    for (int i = 0; i < NUM_QUEENS; i++)
    {
        printf("\n - Enter Row For Queen: %d : ", i + 1);
        scanf("%d", &row);
        if (row > BOARD_WIDTH || row < 1)
        {
            fprintf(stderr, "\n\n - Error: must be in the rage 1 - %d\n\n", BOARD_WIDTH);
            return 0;
        }

        printf("\n - Enter Col For Queen: %d : ", i + 1);
        scanf("%d", &col);
        if (col > BOARD_WIDTH || col < 1)
        {
            fprintf(stderr, "\n\n - Error: must be in the rage 1 - %d\n\n", BOARD_WIDTH);
            return 0;
        }

        board[row - 1][col - 1] = '1';
    }
    return 1;
}


/// @brief Thread startup routine to check all elements of a given row in the board.
/// @param arg board_args struct containing the row to check and board.
/// @returns 0 if at least two queens are found in the row, 1 if zero or one queens are found.
void* check_row(void* arg) {

    board_args *args = (board_args*)arg;
    int* result = malloc(sizeof(int));
    int row = args->i;
    char **board = args->board;
    
    int queen_count = 0;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (board[row][i] == '1')
        {
            queen_count++;
        }
        if (queen_count >= 2)
        {
            *result = 0;
            pthread_exit(result);
        }
    }
    *result = 1;
    pthread_exit(result);
}


/// @brief Thread startup routine to check all elements of a given col in the board.
/// @param arg board_args struct containing the col to check and board.
/// @returns 0 if at least two queens are found in the col, 1 if zero or one queens are found.
void* check_col(void* arg) {

    board_args *args = (board_args*)arg;
    int* result = malloc(sizeof(int));
    int col = args->i;
    char **board = args->board;
    
    int queen_count = 0;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (board[i][col] == '1')
        {
            queen_count++;
        }
        if (queen_count >= 2)
        {
            *result = 0;
            pthread_exit(result);
        }
    }
    *result = 1;
    pthread_exit(result);
}


/// @brief Thread startup routine to check all elements of a given diagonal in the board.
/// @param arg board_args struct containing the row to check and board.
/// @returns 0 if at least two queens are found in a diagonal, 1 if zero or one queens are found.
void* check_diagonal(void* arg) {
    board_args *args = (board_args*)arg;
    int* result = malloc(sizeof(int));
    int row = args->i;
    char **board = args->board;

    if (row == BOARD_WIDTH - 1) {
        free(args);
        *result = 1;
        pthread_exit(result);
    }

    for (int i = 0; i < BOARD_WIDTH; i++) {
        if (board[row][i] == '1') {
            if (i != 0) {
                if (board[row + 1][i - 1] == '1') {
                    *result = 0;
                    pthread_exit(result);
                }
            }

            if (i != BOARD_WIDTH - 1) {
                if (board[row + 1][i + 1] == '1') {
                    *result = 0;
                    pthread_exit(result);
                }
            }
        }
    }
    *result = 1;
    pthread_exit(result);
}


/// @brief Creator of threads to check each row.
/// @returns 0 if at least one thread returns 0, otherwise 1.
int search_rows(board_args *Board) {

    pthread_t th_arr[BOARD_WIDTH];
    int *res = 0;

    for (int i=0; i < BOARD_WIDTH; i++) {
        Board->i = i;
        if (pthread_create(&th_arr[i], NULL, check_row, (void *)Board) !=0 ) {
            perror("pthread no created");
        }

        pthread_join(th_arr[i], (void **)&res);
        if (*res == 0) {
            free(res);
            return 0;
        } 
    }
    free(res);
    return 1;
}


/// @brief Creator of threads to check each col.
/// @returns 0 if at least one thread returns 0, otherwise 1.
int search_cols(board_args *Board) {

    pthread_t th_arr[BOARD_WIDTH];
    int *res = 0;

    for (int i=0; i < BOARD_WIDTH; i++) {
        Board->i = i;
        if (pthread_create(&th_arr[i], NULL, check_col, (void *)Board) !=0 ) {
            perror("pthread no created");
        }

        pthread_join(th_arr[i], (void **)&res);
        if (*res == 0) {
            free(res);
            return 0;
        } 
    }
    free(res);
    return 1;
}


/// @brief Creator of threads to check each diagonal.
/// @returns 0 if at least one thread returns 0, otherwise 1.
int search_diagonals(board_args *Board) {

    pthread_t th_arr[BOARD_WIDTH];
    int *res;

    for (int i=0; i < BOARD_WIDTH; i++) {
        Board->i = i;
        if (pthread_create(&th_arr[i], NULL, check_diagonal, (void *)Board) !=0 ) {
            perror("pthread no created");
        }

        pthread_join(th_arr[i], (void **)&res);
        if (*res == 0) {
            free(res);
            return 0;
        } 
    }
    free(res);
    return 1;
}


int main()
{   
    char **orig_board = create_board();
    board_args *Board = (board_args*)malloc(sizeof(board_args));;
    Board->i=0;
    Board->board = orig_board; 
    
    print_board(Board->board);

    int valid = 0;
    while (valid == 0)
    {
        valid = get_queens(Board->board);
    }

    print_board(Board->board);
    
    if (search_rows(Board) == 0) {
        printf("\n\n -- Two Queens In Row -- \n\n -- Invalid Board --\n\n\n");
        free(Board);
        destroy_board(orig_board);
        return 0;
    }

    if (search_cols(Board) == 0) {
        printf("\n\n -- Two Queens In Col -- \n\n -- Invalid Board --\n\n\n");
        free(Board);
        destroy_board(orig_board);
        return 0;
    }

    if (search_diagonals(Board) == 0) {
        printf("\n\n -- Two Queens In Diagonal --\n\n -- Invalid Board --\n\n\n");
        free(Board);
        destroy_board(orig_board);
        return 0;
    }

    printf("\n\n -- Board Is Valid -- \n\n\n");

    free(Board);
    destroy_board(orig_board);
    return 1;
}
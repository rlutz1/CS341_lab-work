/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

char blocking_attempt[] = "Blocking Attempt";
void blocker(int M, int N, int A[N][M], int B[M][N]) {

   
    int last_size = N; // we are assuming M == N here!
    int row;
    int col; 
    // int for_row = 0;
    // int for_col = 0;

     for (int log = (N / 2); log > 0; log /= 2) { // N = 8, iterate 4, 2, 1 -- 3 times
        printf("log is %d\n", log);

        // will iterate 1, then 2, then 4...
        for (int starting_place = 0; starting_place < N - 1; starting_place += last_size) { // size to jump for next block

          for (int row_counter = 0; row_counter < log; row_counter++) { // needs to iterate 4 times
          // row = log + jumper + (test % log); // + col _ shifter
        
              printf("starter is %d\n", starting_place);

              // always start with log, offset by the jump, and through how many "last rows" we have
              row = log + starting_place + (row_counter % log); 

              // now, the columns...
              // i guess we can just have its own jumper
              for (int col_jumper = 0; col_jumper < N - 1; col_jumper += last_size) { // size to jump for next block
                for(int col_counter = 0; col_counter < log; col_counter++) { // 0 -> 4, 0 -> 2, 0 -> 1
                    printf("iterator inner is %d\n", col_counter);
                      
                    col = col_jumper + (col_counter % log);
                      
                    printf("ACCESSING B: (%d, %d)\n", row, col);
                    B[row][col] = A[col][row];
                    B[col][row] = A[row][col];
                  // for_col++;
                }
                // for_col = 0;
              // // test++
              // for_row++; 
              }
            }
            // test = 0;
            // for_row = 0;
            
        }
        last_size /= 2;
        
    }


    for (int i = 0; i < N; i++) {
      B[i][i] = A[i][i];
    }

} // end method

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

    /* Roxanne fooling around */
    registerTransFunction(blocker, blocking_attempt); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}


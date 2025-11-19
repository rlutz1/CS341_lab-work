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
  int diag;

    for (i = 0; i < N; i++) { // backwards rows
      diag = A[i][i];
      for (j = 0; j < M; j++) { // backward cols
          if (i != j) {
            tmp = A[i][j];
            B[j][i] = tmp;
          }
      }
      B[i][i] = diag;
    }

// int i, j, tmp;
//   int diag;

//     for (i = N - 1; i >= 0; i--) { // backwards rows
//       diag = A[i][i];
//       for (j = M - 1; j >= 0; j--) { // backward cols
//           if (i != j) {
//             tmp = A[i][j];
//             B[j][i] = tmp;
//           }
//       }
//       B[i][i] = diag;
//     }

// does a teensy worse as expected
//  int i, j, tmp;

//     for (i = 0; i < N; i++) {
//         for (j = 0; j < M; j++) {
//             if (i != j) {
//                 tmp = A[i][j];
//                 B[j][i] = tmp;
//             }
            
//         }
//     } 

    // for (i = 0; i < N; i++) {
    //     tmp = A[i][i];
    //     B[i][i] = tmp;
    // }


    // int i, j, tmp;

    // for (i = 0; i < N; i++) {
    //     for (j = 0; j < M; j++) {
    //         tmp = A[i][j];
    //         B[j][i] = tmp;
    //     }
    // } 
    // for (i = 0; i < N; i++) {
    //     for (j = 0; j < M; j++) {
    //         tmp = A[i][j];
    //         B[j][i] = tmp;
    //     }
    // }    

}

/**
 * BAD ATTEMPT
 * has WAY more accesses in general, which is a problem.
 */
char blocking_attempt[] = "Blocking Attempt";
void blocker(int M, int N, int A[N][M], int B[M][N]) {

    int last_size = N; // we are assuming M == N here!
    // int last_size = N / (32 / 4); // N / 8
    int row;
    int col; 
    int temp; // adding temp makes no difference.

     for (int log = (N / 2); log > 0; log /= 2) { // N = 8, iterate 4, 2, 1 -- 3 times

        // will iterate 1, then 2, then 4...
        for (int starting_place = 0; starting_place < N - 1; starting_place += last_size) { // size to jump for next block

          for (int row_counter = 0; row_counter < log; row_counter++) { // needs to iterate 4 times

              // always start with log, offset by the jump, and through how many "last rows" we have
              row = log + starting_place + (row_counter % log); 

              // now, the columns...
              // i guess we can just have its own jumper
              for (int col_jumper = 0; col_jumper < N - 1; col_jumper += last_size) { // size to jump for next block
                for(int col_counter = 0; col_counter < log; col_counter++) { // 0 -> 4, 0 -> 2, 0 -> 1
                      
                    col = col_jumper + (col_counter % log);
                    temp = A[col][row];
                    B[row][col] = temp;
                    
                }
              }
            //   B[row][row] = A[row][row];
            }
        }
        last_size /= 2;
    }

    // last_size = N / (32 / 4); // N / 8
    last_size = N;

    for (int log = (N / 2); log > 0; log /= 2) { // N = 8, iterate 4, 2, 1 -- 3 times

        // will iterate 1, then 2, then 4...
        for (int starting_place = 0; starting_place < N - 1; starting_place += last_size) { // size to jump for next block

          for (int row_counter = 0; row_counter < log; row_counter++) { // needs to iterate 4 times

              // always start with log, offset by the jump, and through how many "last rows" we have
              row = log + starting_place + (row_counter % log); 

              // now, the columns...
              // i guess we can just have its own jumper
              for (int col_jumper = 0; col_jumper < N - 1; col_jumper += last_size) { // size to jump for next block
                for(int col_counter = 0; col_counter < log; col_counter++) { // 0 -> 4, 0 -> 2, 0 -> 1
                      
                    col = col_jumper + (col_counter % log);
                    temp = A[row][col]; 
                    B[col][row] = temp; // i think this could be a major problem.
                    
                }
              }
            }
        }
        last_size /= 2;
    }

    // don't like this
    for (int i = 0; i < N; i++) {
      B[i][i] = A[i][i];
    }

} // end method

char best_32_func[] = "Best For 32";
void best_32(int M, int N, int A[N][M], int B[M][N]) {
    int blocksize = 8;
    int temp;
     // this goes left -> right
    for (int i = 0; i < N; i += blocksize) { // row block increaser
        for (int j = 0; j < M; j += blocksize) { // col block increaser
            if (i == j) {
                for (int ii = i; ii < i + blocksize; ii++) {
                    temp = A[ii][ii]; // load up A
                    for (int jj = j; jj < j + blocksize; jj++) {
                        if (jj != ii) 
                        B[jj][ii] = A[ii][jj];
                    }
                    B[ii][ii] = temp; // load up b for next it
                }
            } else {
                for (int ii = i; ii < i + blocksize; ii++) {
                    for (int jj = j; jj < j + blocksize; jj++) {
                        B[jj][ii] = A[ii][jj];
                    }
                }
            }
        }
    }
}


char best_64_func[] = "Best For 64";
void best_64(int M, int N, int A[N][M], int B[M][N]) {
    int blocksize = 4; // seems like out incers don't actually matter, this DOES
    
    // this goes left -> right
    for (int j = 0; j < M; j += blocksize) { // col block increaser
        for (int i = 0; i < N; i += blocksize) { // row block increaser
     
            for (int ii = i; ii < i + blocksize; ii++) {
                for (int jj = j; jj < j + blocksize; jj++) {
                    B[jj][ii] = A[ii][jj];
                }
            }
        }
    }
//   int blocksize = 4;
//   int inc = 64;

//   for (int j_inc = 0; j_inc < M; j_inc += inc) {
//     for (int j = j_inc; j < j_inc + inc; j += blocksize) { // col block increaser

//         for (int i_inc = 0; i_inc < N; i_inc += inc) {
//             for (int i = i_inc; i < i_inc + inc; i += blocksize) { // row block increaser
          
//                 for (int ii = i; ii < i + blocksize; ii++) {
//                     for (int jj = j; jj < j + blocksize; jj++) {
//                         B[jj][ii] = A[ii][jj];
//                     }
//                 }

//             }
//         }
        
//     }
//   }

    

    // for (int j = 0; j < 32; j += blocksize) { // col block increaser
    //     for (int i = 32; i < 64; i += blocksize) { // row block increaser
        
    //         for (int ii = i; ii < i + blocksize; ii++) {
    //             for (int jj = j; jj < j + blocksize; jj++) {
    //                 B[jj][ii] = A[ii][jj];
    //             }
    //         }
    //     }
    // }

    // for (int j = 32; j < 64; j += blocksize) { // col block increaser
    //     for (int i = 0; i < 32; i += blocksize) { // row block increaser    
          
    //         for (int ii = i; ii < i + blocksize; ii++) {
    //             for (int jj = j; jj < j + blocksize; jj++) {
    //                 B[jj][ii] = A[ii][jj];
    //             }
    //         }
    //     }
    // }
    
    // for (int j = 32; j < 64; j += blocksize) { // col block increaser
    //     for (int i = 32; i < 64; i += blocksize) { // row block increaser
       
    //         for (int ii = i; ii < i + blocksize; ii++) {
    //             for (int jj = j; jj < j + blocksize; jj++) {
    //                 B[jj][ii] = A[ii][jj];
    //             }
    //         }
    //     }
    // }

}


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
    // registerTransFunction(blocker, blocking_attempt); 
    registerTransFunction(best_32, best_32_func); 
    registerTransFunction(best_64, best_64_func); 
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


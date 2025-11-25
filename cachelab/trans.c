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
void trans(int M, int N, int A[N][M], int B[M][N]);
void best_32(int M, int N, int A[N][M], int B[M][N]);
void best_64(int M, int N, int A[N][M], int B[M][N]);
void best_61_67(int M, int N, int A[N][M], int B[M][N]);
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
  if (M == 32 && N == 32) { // optimized 32x32 output size
    best_32(M, N, A, B);
  } else if (M == 64 && N == 64) { // optimized 64x64 output size
    best_64(M, N, A, B);
  } else if (M == 61 && N == 67) { // optimized 61x67 output size
    best_61_67(M, N, A, B);
  } else { // given anything else, no optimization built.
    trans(M, N, A, B);
  } // end if
} // end submission function

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
  int i, j;
  for (i = 0; i < N; i++) { 
    for (j = 0; j < M; j++) { 
      B[j][i] = A[i][j];
    }
  }
}

/**
 * ====================================================
 * FUNCTIONS WRITTEN BY HUMAN OPTIMIZED FOR EACH SIZE
 * 
 * note: please read documentation in report for more
 * thorough discussion on the approach. header comments
 * will give a brief summation only.
 * ====================================================
 */


/**
 * 
 */
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
                        if (jj != ii) {
                            B[jj][ii] = A[ii][jj];
                        }
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

 // for (ii = i + block_jump_right; ii > i - 1 ; ii--) {
                //     temp = A[ii][ii]; // load up A
                //     for (jj = j + block_jump_down; jj < j - 1; jj--) {
                //         if (jj != ii)  {
                //             B[jj][ii] = A[ii][jj];
                //         }
                //     }
                //     B[ii][ii] = temp; // load up b for next it
                // }

int isDiagonal(int i, int j) {
  return (
    ((i < 8 && i >= 0) && (j < 8 && j >= 0)) ||
    ((i < 16 && i >= 8) && (j < 16 && j >= 8)) ||
    ((i < 24 && i >= 16) && (j < 24 && j >= 16)) ||
    ((i < 32 && i >= 24) && (j < 32 && j >= 24)) ||
    ((i < 40 && i >= 32) && (j < 40 && j >= 32)) ||
    ((i < 48 && i >= 40) && (j < 48 && j >= 40)) ||
    ((i < 56 && i >= 48) && (j < 56 && j >= 48)) ||
    ((i < 64 && i >= 56) && (j < 64 && j >= 56)) 
  );
}

char best_64_func[] = "Best For 64";
void best_64(int M, int N, int A[N][M], int B[M][N]) {
 // DIAGS -----------------------------------------------------------
 int blocksize = 8; int i; int j; int ii; int jj; int diag_row_fill; int diag_col_fill; 


  // we're gonna fill a little differently. instead we are going
  // to in place put the top row from A into top right row of B
  // and bottom row into one to the left of the other in B is well.
  // THEN, we will fill B diag rowwise in the needed transpose order.
  // the following does the first half of the matrix
  blocksize = 4;
  for (i = 0, j = 0; i < (N / 2); i += 8, j += 8) {
    // fill top row from A into top right rows
    for (ii = i, diag_row_fill = 0; ii < i + 4; ii++, diag_row_fill++){
      for (jj = j, diag_col_fill = 56; jj < j + 8; jj++, diag_col_fill++) {
        B[diag_row_fill][diag_col_fill] = A[ii][jj]; // LAST 8 col of B, 56 -> 63
      }
    }

    // fill next row OVER to left with bottom half from A
    for (ii = i + 4, diag_row_fill = 0; ii < i + 8; ii++, diag_row_fill++){
      for (jj = j, diag_col_fill = 48; jj < j + 8; jj++, diag_col_fill++) {
        B[diag_row_fill][diag_col_fill] = A[ii][jj]; // LAST 8 col of B, 56 -> 63
      }
    }

    // THEN, we fill B accordingly and row wise by 4x4 blocks from B rows

    for (int ii = i, diag_row_fill = 0; ii < i + blocksize; ii++, diag_row_fill++) { // fill the upper left square
      for (int jj = j, diag_col_fill = 56; jj < j + blocksize; jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }

    for (int ii = i + blocksize, diag_row_fill = 0; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the upper right square
      for (int jj = j, diag_col_fill = 48; jj < j + blocksize; jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }


    for (int ii = i, diag_row_fill = 0; ii < i + blocksize; ii++, diag_row_fill++) { // fill the bottom left square
      for (int jj = j + blocksize, diag_col_fill = 60; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }

    for (int ii = i + blocksize, diag_row_fill = 0; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the bottom right square
      for (int jj = j + blocksize, diag_col_fill = 52; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }
   
  }

  // THEN, we do the other half diagonals. which means we use the bottom left rows of B 
  // instead and fill the same way.

  blocksize = 4;
  for (i = (N / 2), j = (N / 2); i < N; i += 8, j += 8) {
    // fill top row from A into BOTTOM LEFT rows
    for (ii = i, diag_row_fill = 60; ii < i + 4; ii++, diag_row_fill++){
      for (jj = j, diag_col_fill = 0; jj < j + 8; jj++, diag_col_fill++) {
        B[diag_row_fill][diag_col_fill] = A[ii][jj]; // LAST 8 col of B, 56 -> 63
      }
    }

    // fill next row OVER to RIGHT with bottom half from A
    for (ii = i + 4, diag_row_fill = 60; ii < i + 8; ii++, diag_row_fill++){
      for (jj = j, diag_col_fill = 8; jj < j + 8; jj++, diag_col_fill++) {
        B[diag_row_fill][diag_col_fill] = A[ii][jj]; // LAST 8 col of B, 56 -> 63
      }
    }

    // THEN, we fill B accordingly and row wise by 4x4 blocks from B rows

    for (int ii = i, diag_row_fill = 60; ii < i + blocksize; ii++, diag_row_fill++) { // fill the upper left square
      for (int jj = j, diag_col_fill = 0; jj < j + blocksize; jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }

    for (int ii = i + blocksize, diag_row_fill = 60; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the upper right square
      for (int jj = j, diag_col_fill = 8; jj < j + blocksize; jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }


    for (int ii = i, diag_row_fill = 60; ii < i + blocksize; ii++, diag_row_fill++) { // fill the bottom left square
      for (int jj = j + blocksize, diag_col_fill = 4; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }


    for (int ii = i + blocksize, diag_row_fill = 60; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the bottom right square
      for (int jj = j + blocksize, diag_col_fill = 12; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }
   
  }


  // GENERAL CASE --------------------------------------------------------

  blocksize = 4;

  // try the sideways U pattern to fill with minimal misses generally.
  for (i = 0; i < N; i += 8) { // inc by 8 blocks here
    for (j = 0; j < M; j += 8) {
      if (!isDiagonal(i, j)) { // PLEASE don't redo the diagonals lol
        // upper left

         for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
          for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

        for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
          for (int jj = j; jj < j + blocksize; jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

       
        for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
          for (int jj = j; jj < j + blocksize; jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }
        

        for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
          for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

        
      }
    }
  }

}


char best_67_61_func[] = "Best For 61x67";
void best_61_67(int M, int N, int A[N][M], int B[M][N]) {

  int i; int j; int ii; int jj;
  int blocksize = 8;

  for (j = 0; j < M; j += blocksize) {
    for (i = 0; i < N; i += blocksize) {
    
     
      for (ii = i; ii < i + blocksize; ii++) {
        for (jj = j; jj < j + blocksize; jj++) {
          if (ii < N && jj < M) {
            B[jj][ii] = A[ii][jj];
          }
        }
      }


    }
  }


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
    registerTransFunction(best_61_67, best_67_61_func); 
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


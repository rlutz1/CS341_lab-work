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

     // new attempt
  int blocksize = 8; // first to 8 for the diags
  int i; int j; int ii; int jj;

  // step 1: handle the diagonals.

  for (i = 0; i < N - blocksize; i += blocksize) {
    for (ii = i; ii < i + blocksize; ii++){
      for (jj = i; jj < i + blocksize; jj++) {
        B[ii][jj + blocksize] = A[ii][jj];
      }
    }
  } 


  // avoid final problem by putting to left
  for (ii = N - blocksize; ii < N; ii++) {
    for (jj = N - blocksize; jj < N; jj++) {
      B[ii][jj - blocksize] = A[ii][jj];
    }
  }

  

  // now, let's fill the diagonals FROM B
  // we're gonna fill as a square shape starting in top left
blocksize = 4;
  for (i = 0, j = 0; i < N; i += 8, j += 8) {

    if (i < M - 8) { // NOT the last row  
      for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
        for (int jj = j; jj < j + blocksize; jj++) {
            B[jj][ii] = B[ii][jj + 8];
        }
      }

      for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
        for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
            B[jj][ii] = B[ii][jj + 8];
        }
      }

        for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
        for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
            B[jj][ii] = B[ii][jj + 8];
        }
      }

      for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
        for (int jj = j; jj < j + blocksize; jj++) {
            B[jj][ii] = B[ii][jj + 8];
        }
      }
    } else {

      for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
        for (int jj = j; jj < j + blocksize; jj++) {
            B[jj][ii] = B[ii][jj - 8];
        }
      }

      for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
        for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
            B[jj][ii] = B[ii][jj - 8];
        }
      }

      for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
        for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
            B[jj][ii] = B[ii][jj - 8];
        }
      }

      for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
        for (int jj = j; jj < j + blocksize; jj++) {
            B[jj][ii] = B[ii][jj - 8];
        }
      }
    }
  }


  blocksize = 4;

  // try the square pattern used above
  for (i = 0; i < N; i += 8) { // inc by 8 blocks here
    for (j = 0; j < M; j += 8) {
      if (!isDiagonal(i, j)) { // PLEASE don't redo the diagonals lol
        // upper left 
        for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
          for (int jj = j; jj < j + blocksize; jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

        for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
          for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

        for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
          for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

        for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
          for (int jj = j; jj < j + blocksize; jj++) {
              B[jj][ii] = A[ii][jj];
          }
        }

      }
    }
  }







//   int blocksize = 8;
// int i; int j; int ii; int jj;
// // int temp;

// // STEP 1, place the contents of Diags of A into right shifted diags of B
// for (i = 0; i < N - blocksize; i += blocksize) {
//   for (ii = i; ii < i + blocksize; ii++){
//     for (jj = i; jj < i + blocksize; jj++) {
//       B[ii][jj + blocksize] = A[ii][jj];
//     }
//   }
// } 

//   // avoid final problem by putting to left
//    for (ii = N - blocksize; ii < N; ii++) {
//       for (jj = N - blocksize; jj < N; jj++) {
//         B[ii][jj - 8] = A[ii][jj];
//       }
//    }



// blocksize = 4;


//  for (i = 0; i < N; i += blocksize) { // row block increaser
//       for (j = 0; j < M; j += blocksize) { // col block increaser
          
//           // if (i == j || (j - blocksize) == i || (i - blocksize) == j) { // DIAGONAL 8 BLOCK
//           if (isDiagonal(i, j)) { // for now since buggy

//             if (j < M - 8) { // NOT the last row
//               for (int ii = i; ii < i + blocksize; ii++) { // reach ahead and pull from B!
//                   for (int jj = j; jj < j + blocksize; jj++) {
//                       B[jj][ii] = B[ii][jj + 8];
//                   }
//               }
//             } else { // mitigate damage on final diag block
//                  for (int ii = i; ii < i + blocksize; ii++) {
//                 //   temp = A[ii][ii]; // load up A
//                   for (int jj = j; jj < j + blocksize; jj++) {
//                     //   if (jj != ii) {
//                         B[jj][ii] = B[ii][jj - 8];
//                     //   }
//                   }
//                 //   B[ii][ii] = temp; // load up b for next it
//               }
//             }
//             // printf("diag at (%d, %d)\n", i, j);
           
//           }
       
//       }
  
//     }



// // STEP 3, somewhat normal transpose for all cases except diagonal

//    for (i = 0; i < N; i += blocksize) { // row block increaser
//       for (j = 0; j < M; j += blocksize) { // col block increaser
          
//           // if (i == j || (j - blocksize) == i || (i - blocksize) == j) { // DIAGONAL 8 BLOCK
//           if (!isDiagonal(i, j)) { // for now since buggy

//               for (int ii = i; ii < i + blocksize; ii++) {
//                   for (int jj = j; jj < j + blocksize; jj++) {
//                       B[jj][ii] = A[ii][jj];
//                   }
//               }
//           }
       
//       }
  
//     }



// int blocksize = 4;
// //  int temp;
//  int i;
//  int j; 
//  int ii;
//  int jj;
//  int ff = 0;
//  int a_row; int a_col; 

//   // STEP 1, place the contents of Diags of A into right shifted diags of B
//   for (i = 0; i < N - 8; i += 8) {
//     for (ii = i; ii < i + 8; ii++){
//       for (jj = i; jj < i + 8; jj++) {
//         B[ii][jj + 8] = A[ii][jj];
//       }
//     }
//   }

//   // avoid final problem by putting to left
//    for (ii = N - 8; ii < N; ii++) {
//       for (jj = N - 8; jj < N; jj++) {
//         B[ii][jj - 8] = A[ii][jj];
//       }
//    }
 

// //    // do the lower half in a zig zag pattern
// //  // need to iterate 2 * N times, then - 2, then - 2
// //   // need to start in upper left block, then down, then right, then down, then right....
// //   // and a has to be opposite.
// //   ff = 0;
// //   a_row = 0; a_col = 0; i = N; 

// //   while (a_col < i) {
// //       // transpose this block
// //       for (ii = a_row; ii < a_row + blocksize; ii++){
// //           for (jj = a_col; jj < a_col + blocksize; jj++) {
// //               // B[jj][ii] = A[ii][jj];
// //               if ((a_row >= 56 && a_row < 64) && (a_col >= 56 && a_col < 64)) {
// //                   B[jj][ii] = B[ii][jj - 8];
// //               } else {
// //                     B[jj][ii] = B[ii][jj + 8];
// //               }
              
// //           }
// //       }

// //       if (ff == 0) {
// //           a_col += blocksize;
// //           ff = 1;
// //       } else {  
// //           a_row += blocksize;
// //           ff = 0;
// //       }
// //   }


// //   ff = 0;
// //   a_row = 0; a_col = 0; i = N; 
  
// //     while (a_row < i) {
     
// //         if (a_row == a_col) {
          
// //           if (ff == 0) {
// //             a_row += blocksize;
// //             ff = 1;
// //           } else {  
// //             a_col += blocksize;
// //             ff = 0;
// //           }
// //           continue;
// //         }
// //       // transpose this block
// //       for (ii = a_row; ii < a_row + blocksize; ii++){
// //         for (jj = a_col; jj < a_col + blocksize; jj++) {
// //          // B[jj][ii] = A[ii][jj];
// //             if ((a_row >= 56 && a_row < 64) && (a_col >= 56 && a_col < 64)) {
// //                 B[jj][ii] = B[ii][jj - 8];
// //             } else {
// //                 B[jj][ii] = B[ii][jj + 8];
// //             }
// //         }
// //       }

// //       if (ff == 0) {
// //         a_row += blocksize;
// //         ff = 1;
// //       } else {  
// //         a_col += blocksize;
// //         ff = 0;
// //       }
// //     }
 

//   // handle diagonals SPECIFICALLY
//   for (i = 0; i < N; i += blocksize) { // row block increaser
//     for (j = 0; j < M; j += blocksize) { // col block increaser
        
//       if (isDiagonal(i, j)) { // for now since buggy

//         if (j < M - 8) { // NOT the last row
//           for (int ii = i; ii < i + blocksize; ii++) { // reach ahead and pull from B!
//               for (int jj = j; jj < j + blocksize; jj++) {
//                   B[jj][ii] = B[ii][jj + 8];
//               }
//           }
//         } else { // mitigate damage on final diag block
//             for (int ii = i; ii < i + blocksize; ii++) { // reach ahead and pull from B!
//               for (int jj = j; jj < j + blocksize; jj++) {
//                   B[jj][ii] = B[ii][jj - 8];
//               }
//             }
//           }
//         }
//       } 
//     }
  






//  // do the lower half in a zig zag pattern
//  // need to iterate 2 * N times, then - 2, then - 2
//   // need to start in upper left block, then down, then right, then down, then right....
//   // and a has to be opposite.
//   ff = 0;
//   a_row = 0; a_col = 8; i = N; 
//   for (int k = 2; k <= (N / 8); k++) {
//     while (a_col < i) {
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//         for (jj = a_col; jj < a_col + blocksize; jj++) {
//           B[jj][ii] = A[ii][jj];
//         }
//       }

//       if (ff == 0) {
//         a_col += blocksize;
//         ff = 1;
//       } else {  
//         a_row += blocksize;
//         ff = 0;
//       }
//     }
//     ff = 0;
//     a_row = 0;
//     a_col = 8 * k;
//   }

//   ff = 0;
//   a_row = 8; a_col = 0; i = N; 
//   for (int k = 2; k <= (N / 8); k++) {
//     while (a_row < i) {
     
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//         for (jj = a_col; jj < a_col + blocksize; jj++) {
//           B[jj][ii] = A[ii][jj];
//         }
//       }

//       if (ff == 0) {
//         a_row += blocksize;
//         ff = 1;
//       } else {  
//         a_col += blocksize;
//         ff = 0;
//       }
//     }
//     ff = 0;
//     a_row = 8 * k;
//     a_col = 0;
//   }

//   // final edge case for now: need to fill in the weird gaps
//    a_row = 8; a_col = 4;
//    for (int k = 0; k < 7; k++) {
//     for (ii = a_row; ii < a_row + blocksize; ii++) {
//       for (jj = a_col; jj < a_col + blocksize; jj++) {
//          B[jj][ii] = A[ii][jj];
//       }
//     }

//     for (ii = a_row; ii < a_row + blocksize; ii++) {
//       for (jj = a_col; jj < a_col + blocksize; jj++) {
//         B[ii][jj] = A[jj][ii];
//       }
//     }
//     a_row += 8; a_col += 8;
//    }

 

    
//  int blocksize = 4;
// //  int temp;
//  int i;
// //  int j; 
//  int ii;
//  int jj;
//  int ff = 0;
//  int a_row;  int a_col; //int b_col;int b_row;
//  // do the lower half in a zig zag pattern
//  // need to iterate 2 * N times, then - 2, then - 2
//   // need to start in upper left block, then down, then right, then down, then right....
//   // and a has to be opposite.
//   a_row = 0; a_col = 0;  i = N; 
//   for (int k = 1; k <= (N / 8); k++) {
//     while (a_col < i) {
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//         for (jj = a_col; jj < a_col + blocksize; jj++) {
//           B[jj][ii] = A[ii][jj];
//         }
//       }

//       if (ff == 0) {
//         a_col += blocksize;
//         ff = 1;
//       } else {  
//         a_row += blocksize;
//         ff = 0;
//       }
//     }
//     ff = 0;
//     a_row = 0;
//     a_col = 8 * k;
//   }

//   ff = 0;
//   a_row = 4; a_col = 0;  i = N; 
//   for (int k = 1; k <= (N / 8); k++) {
//     while (a_row < i) {
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//         for (jj = a_col; jj < a_col + blocksize; jj++) {
//           B[jj][ii] = A[ii][jj];
//         }
//       }

//       if (ff == 0) {
//         a_row += blocksize;
//         ff = 1;
//       } else {  
//         a_col += blocksize;
//         ff = 0;
//       }
//     }
//     ff = 0;
//     a_row = 8 * k;
//     a_col = 0;
//   }
    
//  int blocksize = 8;
//  int temp;
//  int i;
//  int j; 
//  int ii;
//  int jj;





    // int blocksize = 8;
    // int temp;
    //  // this goes left -> right
    // for (int i = 0; i < N; i += blocksize) { // row block increaser
    //     for (int j = 0; j < M; j += blocksize) { // col block increaser
            
    //         if (i == j) {
    //             for (int ii = i; ii < i + (blocksize / 2); ii++) {
    //                 temp = A[ii][ii]; // load up A
    //                 for (int jj = j; jj < j + (blocksize / 2); jj++) {
    //                     if (jj != ii) {
    //                         B[jj][ii] = A[ii][jj];
    //                     }
    //                 }
    //                 B[ii][ii] = temp; // load up b for next it
    //             }
    //         } else {
    //             for (int ii = i; ii < i + (blocksize / 2); ii++) {
    //                 for (int jj = j; jj < j + (blocksize / 2); jj++) {
    //                     B[jj][ii] = A[ii][jj];
    //                 }
    //             }
    //         }
           
    //     }
    // }

    // for (int i = 4; i < N; i += blocksize) { // row block increaser
    //     for (int j = 4; j < M; j += blocksize) { // col block increaser
            
    //         if (i == j) {
    //             for (int ii = i; ii < i + (blocksize / 2); ii++) {
    //                 temp = A[ii][ii]; // load up A
    //                 for (int jj = j; jj < j + (blocksize / 2); jj++) {
    //                     if (jj != ii) {
    //                         B[jj][ii] = A[ii][jj];
    //                     }
    //                 }
    //                 B[ii][ii] = temp; // load up b for next it
    //             }
    //         } else {
    //             for (int ii = i; ii < i + (blocksize / 2); ii++) {
    //                 for (int jj = j; jj < j + (blocksize / 2); jj++) {
    //                     B[jj][ii] = A[ii][jj];
    //                 }
    //             }
    //         }
           
    //     }
    // }

 // if (i == j || i + 1 == j || j + 1 == i) {
            //     int step = 1;
            //     if (j % 8 != 0) {
            //         step = 2;
            //     }
            //     if (j + blocksize + step < N) {
            //         for (int ii = i; ii < i + blocksize; ii++) {
            //             for (int jj = j; jj < j + blocksize; jj++) {
            //                 B[ii][jj  + step] = A[ii][jj];
            //             }
            //         }
            //         for (int ii = i; ii < i + blocksize; ii++) {
            //             for (int jj = j; jj < j + blocksize; jj++) {
            //                 B[jj][ii] = B[ii][jj + step];
            //             }
            //         }
            //     } else {
            //         for (int ii = i; ii < i + blocksize; ii++) {
                    
            //             temp = A[ii][ii]; // load up A
            //             for (int jj = j; jj < j + blocksize; jj++) {
            //                 if (jj != ii) {
            //                     B[jj][ii] = A[ii][jj];
            //                 }
            //             }
            //             B[ii][ii] = temp; // load up b for next it
            //         }
            //     }
                
                
            // } else {
            //     for (int ii = i; ii < i + blocksize; ii++) {
            //         for (int jj = j; jj < j + blocksize; jj++) {
            //             B[jj][ii] = A[ii][jj];
            //         }
            //     }
            // }


    // int blocksize = 4;
    // int block_jump_right = 4;
    // int block_jump_down = 4;
    // int i;
    // int j;
    // int ii;
    // int jj;
    // int temp;
    //  // this goes left -> right
   
    
    // for (i = 0; i < N; i += block_jump_right) { // row block increaser
    //     for (j = 0; j < M; j += block_jump_down) { // col block increaser 
            
    //         // if (i - 1 == j + 1) { // theres a diagonal below me
    //         //     // handle this row
    //         //     for (ii = i; ii < i + block_jump_right; ii++) {
    //         //         temp = A[ii][ii]; // load up A
    //         //         for (jj = j; jj < j + block_jump_down; jj++) {
    //         //             if (jj != ii)  {
    //         //                 B[jj][ii] = A[ii][jj];
    //         //             }
    //         //         }
    //         //         B[ii][ii] = temp; // load up b for next it
    //         //     }
    //         //     // deal with the diagonal
    //         //     j += block_jump_down;
    //         //     for (ii = i; ii < i + block_jump_right; ii++) {
    //         //         temp = A[ii][ii]; // load up A
    //         //         for (jj = j; jj < j + block_jump_down; jj++) {
    //         //             if (jj != ii)  {
    //         //                 B[jj][ii] = A[ii][jj];
    //         //             }
    //         //         }
    //         //         B[ii][ii] = temp; // load up b for next it
    //         //     }

                
    //         // } else if (j % 8 == 0 && j == i)  { // first diagonal
    //         //     // need to potentially deal with diagonal itself
    //         //     for (ii = i; ii < i + block_jump_right; ii++) {
    //         //         temp = A[ii][ii]; // load up A
    //         //         for (jj = j; jj < j + block_jump_down; jj++) {
    //         //             if (jj != ii)  {
    //         //                 B[jj][ii] = A[ii][jj];
    //         //             }
    //         //         }
    //         //         B[ii][ii] = temp; // load up b for next it
    //         //     }

    //         //     // AND THEN we need to deal with the next row down
    //         //     // which has a similar issue to the diagonal with 
    //         //     // set conflicts
    //         //     j += block_jump_down;
                
    //         //     for (ii = i; ii < i + block_jump_right; ii++) {
    //         //         temp = A[ii][ii]; // load up A
    //         //         for (jj = j; jj < j + block_jump_down; jj++) {
    //         //             if (jj != ii)  {
    //         //                 B[jj][ii] = A[ii][jj];
    //         //             }
    //         //         }
    //         //         B[ii][ii] = temp; // load up b for next it
    //         //     }

               
                
    //         // } else {
    //         //     for (ii = i; ii < i + block_jump_right; ii++) {
    //         //          for (jj = j; jj < j + block_jump_down; jj++) {
    //         //             B[jj][ii] = A[ii][jj];
    //         //         }
    //         //     }
    //         // }
    //     }
    // }

//     int blocksize = 4; // seems like out incers don't actually matter, this DOES
    
//     // this goes left -> right
//     for (int j = 0; j < M; j += blocksize) { // col block increaser
//         for (int i = 0; i < N; i += blocksize) { // row block increaser
     
//             for (int ii = i; ii < i + blocksize; ii++) {
//                 for (int jj = j; jj < j + blocksize; jj++) {
//                     B[jj][ii] = A[ii][jj];
//                 }
//             }
//         }
//     }
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


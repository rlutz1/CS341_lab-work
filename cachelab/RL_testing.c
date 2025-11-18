#include <stdio.h>

void printM(int M, int N, int B[M][N]) {
    for (int i = 0; i < M; i++) { // row block increaser
      for (int j = 0; j < N; j++) { // col block increaser
          printf("%d, ", B[i][j]);
      }
      printf("\n");
    }
}


// best for 32 SO FAR
// running with 344 misses, which is super close
void best_32(int M, int N, int A[N][M], int B[M][N]) {
    // int blocksize = 8;
    int blocksize = 4;

     for (int j = 0; j < M; j += blocksize) { // col block increaser
      for (int i = 0; i < N; i += blocksize) { // row block increaser
     
          
        //// EHHHHHHHHHHHHH, idk that this is possible
          // for (int ii = i, B_row = i + blocksize; ii < i + blocksize; ii++, B_row--) {
          //   for (int jj = j, A_col = j + blocksize; jj < j + blocksize; ++jj, A_col--) {
          
          //         B[B_row][jj] = A[ii][A_col];
          //         printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", B_row, jj, A[ii][A_col]);
          //         printM(M, N, B);
                
                 
          //     }

            for (int ii = i; ii < i + blocksize; ++ii) {
             for (int jj = j; jj < j + blocksize; ++jj) {
          
                  B[jj][ii] = A[ii][jj];
                  printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
                  printM(M, N, B);
                
                 
              }
          }
              // for (int jj = j; jj < j + blocksize; ++jj) {
          
              //     B[jj][ii] = A[ii][jj];
              //     printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
              //     printM(M, N, B);
                
                 
              // }
          }
      }
    

    // // she said something about the diagonals
    // for (int j = 0; j < M; j += blocksize) { // col block increaser
    //   for (int i = 0; i < N; i += blocksize) { // row block increaser
     
          
    //       for (int ii = i; ii < i + blocksize; ++ii) {
    //           for (int jj = j; jj < j + blocksize; ++jj) {
    //             if (ii != jj) {
    //               B[jj][ii] = A[ii][jj];
    //               printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
    //               printM(M, N, B);
    //             }
                 
    //           }
    //       }
    //   }
    // }

    // for (int i = 0; i < N; i++) {
    //   B[i][i] = A[i][i];
    // }

  //  for (int i = 0; i < N; i += blocksize) { // row block increaser
  //   for (int j = 0; j < M; j += blocksize) { // col block increaser
        
  //       for (int ii = i; ii < i + blocksize; ii++) {
  //           for (int jj = j; jj < j + blocksize; jj++) {
  //               B[jj][ii] = A[ii][jj];
  //           }
  //       }
  //   }
  // }

  // int blocksize = 8;

    // this deals with diagonals separately, does slightly worse
    //  // she said something about the diagonals
    // for (int j = 0; j < M; j += blocksize) { // col block increaser
    //   for (int i = 0; i < N; i += blocksize) { // row block increaser
     
          
    //       for (int ii = i; ii < i + blocksize; ++ii) {
    //           for (int jj = j; jj < j + blocksize; ++jj) {
    //             if (ii != jj) {
    //               B[jj][ii] = A[ii][jj];
    //             }
    //           }
    //       }
    //   }
    // }

    // for (int i = 0; i < N; i++) {
    //   B[i][i] = A[i][i];
    // }


//    // this goes left -> right
  //  for (int j = 0; j < M; j += blocksize) { // col block increaser
  //     for (int i = 0; i < N; i += blocksize) { // row block increaser
     
          
  //         for (int ii = i; ii < i + blocksize; ++ii) {
  //             for (int jj = j; jj < j + blocksize; ++jj) {
  //                 B[jj][ii] = A[ii][jj];
  //             }
  //         }
  //     }
  //   }

    // this goes top -> bottom
    // she said something about the diagonals
    // for (int i = 0; i < N; i += blocksize) { // row block increaser
    //   for (int j = 0; j < M; j += blocksize) { // col block increaser
          
    //       for (int ii = i; ii < i + blocksize; ++ii) {
    //           for (int jj = j; jj < j + blocksize; ++jj) {
    //               B[jj][ii] = A[ii][jj];
    //           }
    //       }
    //   }
    // }
}

// work on something here...
void best_64(int M, int N, int A[N][M], int B[M][N]) {
   int blocksize = 8;
  //  int test = 32;
   // try splitting into 2 blocks? of 32 size?
  //  for (int k = 0; k < 4; k++) {
  //   // printf("running\n");

    for (int i = 0; i < 32; i += blocksize) { // row block increaser
        for (int j = 0; j < 32; j += blocksize) { // col block increaser
          
          for (int ii = i; ii < i + blocksize; ii++) {
              for (int jj = j; jj < j + blocksize; jj++) {
                  printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
                  B[jj][ii] = A[ii][jj];
              }
          }
      }
    }

    for (int i = 0; i < 32; i += blocksize) { // row block increaser
        for (int j = 32; j < 64; j += blocksize) { // col block increaser
          
          for (int ii = i; ii < i + blocksize; ii++) {
              for (int jj = j; jj < j + blocksize; jj++) {
                  printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
                  B[jj][ii] = A[ii][jj];
              }
          }
      }
    }

    for (int i = 32; i < 64; i += blocksize) { // row block increaser
        for (int j = 0; j < 32; j += blocksize) { // col block increaser
          
          for (int ii = i; ii < i + blocksize; ii++) {
              for (int jj = j; jj < j + blocksize; jj++) {
                  printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
                  B[jj][ii] = A[ii][jj];
              }
          }
      }
    }

    for (int i = 32; i < 64; i += blocksize) { // row block increaser
        for (int j = 32; j < 64; j += blocksize) { // col block increaser
          
          for (int ii = i; ii < i + blocksize; ii++) {
              for (int jj = j; jj < j + blocksize; jj++) {
                  printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
                  B[jj][ii] = A[ii][jj];
              }
          }
      }
    }
  //  }
   
}


void blocker(int M, int N, int A[N][M], int B[M][N]) {

    int last_size = N; // we are assuming M == N here!
    int row;
    int col; 
    int for_row = 0;
    int for_col = 0;

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


   

    // for (int log = (N / 2); log > 0; log /= 2) { // N = 8, iterate 4, 2, 1 -- 3 times
    //     printf("log is %d\n", log);

        
    //     for (int jumper = 0; jumper < N - 1; jumper += last_size) { // size to jump for next block

    //       for (int grab_per_row = 0; grab_per_row < 4; grab_per_row++) { // needs to iterate 4 times
    //       // row = log + jumper + (test % log); // + col _ shifter
          

    //           printf("jumper is %d\n", jumper);

    //           row = log + jumper + (for_row % log);
    //           // issue, need to stay on that row for longer


    //           for(int col_shifter = 0; col_shifter < 4; col_shifter++) { // 0 -> 4, 0 -> 2, 0 -> 1
    //               printf("iterator inner is %d\n", col_shifter);
                    
    //               col = jumper + (for_col % log);
                    
    //               printf("ACCESSING B: (%d, %d)\n", row, col);
    //               B[row][col] = A[col][row];
    //               B[col][row] = A[row][col];
    //             for_col++;
    //           }
    //           for_col = 0;
    //           // test++
    //           for_row++; 
    //         }
    //         // test = 0;
    //         for_row = 0;
            
    //     }
    //     last_size /= 2;
        
    // }

} // end method

void init(int M, int N, int A[N][M]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      A[i][j] = j;
    }
  }
}

int main() {
  // printf("jsadksnd");
  // int M = 8;
  // int N = 8;
  // int A[8][8] = { {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8},
  //                 {1, 2, 3, 4, 5, 6, 7, 8}
  //               };

  // int B[8][8] = {0};
  // int M = 4; int N = 4;

  // int A[4][4] = {
  //   {1, 2, 3, 4},
  //   {1, 2, 3, 4},
  //   {1, 2, 3, 4},
  //   {1, 2, 3, 4}
  // };

  // int B[4][4] = {0};
  const int size = 64;
  int M = size; int N = size;
  int A[size][size];
  int B[size][size];

  init(M, N, A);

  best_64(M, N, A, B);

   printf("done:\n");
  printM(M, N, B);
  

 
  return 0;
}
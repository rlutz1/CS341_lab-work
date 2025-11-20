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
  int blocksize = 8;

  // // this goes left -> right
  // for (int j = 0; j < M; j += blocksize) { // col block increaser
  //     for (int i = 0; i < N; i += blocksize) { // row block increaser
    
  //         for (int ii = i; ii < i + blocksize; ii++) {
  //             for (int jj = j; jj < j + blocksize; jj++) {
  //                 B[jj][ii] = A[ii][jj];
  //             }
  //         }
  //     }
  //}

  // this goes left -> right
  for (int j = 0; j < M; j += blocksize) { // col block increaser
      for (int i = 0; i < N; i += blocksize) { // row block increaser
        // THIS ASSUMES EVEN DIMENSIONS!!!!!!!!!!!!!
          for (int ii = i; ii < i + blocksize; ii += 2) {
              for (int jj = j; jj < j + blocksize; jj += 2) {
                  B[jj][ii] = A[ii][jj];
                  B[jj][ii + 1] = A[ii + 1][jj];
                  B[jj + 1][ii] = A[ii][jj + 1];
                  B[jj + 1][ii + 1] = A[ii + 1][jj + 1];
              }
          }
      }
  }




  // // copy in A
  //   for (int j = 0; j < M; j++) { // col block increaser
  //     for (int i = 0; i < N; i++) { // row block increaser
  //       B[j][i] = A[j][i];
  //     }
  //   }
  //   int temp;
  //  // this goes left -> right
  // for (int i = 0; i < N; i += blocksize) { // row block increaser
  //   for (int j = 0; j < M; j += blocksize) { // col block increaser
     
    
  //         for (int ii = i; ii < i + blocksize; ii++) {
  //             for (int jj = i + 1; jj < j + blocksize; jj++) {
  //                 temp = B[ii][jj];
  //                 B[ii][jj] = B[jj][ii];
  //                 B[jj][ii] = temp;
  //             }
  //         }
  //     }
  // }

    // int blocksize = 8;
    // int blocksize = 8;
    // int temp;
    // // copy in A
    // for (int j = 0; j < M; j++) { // col block increaser
    //   for (int i = 0; i < N; i++) { // row block increaser
    //     B[j][i] = A[j][i];
    //   }
    // }
    // printM(M, N, B);
    // int control;
    // for (int j = 0; j < M; j += blocksize) { // col block increaser
    //   // for (int i = 0; i < N; i += blocksize) { // row block increaser
    //     control = 0;

        
    //     for (int total_times_in_block = 0; total_times_in_block < blocksize - 1; total_times_in_block++) {
          
    //       for (int b_row = j + 1; b_row < (j + blocksize - control); b_row++) {

    //         for (int b_col = j + control; b_col < (j + blocksize - 1); b_col++) {
            
    //           temp = B[b_row][b_col];
    //           B[b_row][b_col] = B[b_row - 1][b_col + 1];
    //           B[b_row - 1][b_col + 1] = temp;
    //           printM(M, N, B);
    //           printf("\n");
    //         }

    //       }

    //       control++;

    //     }
        

    //         // let's try a different access pattern
    //         // for (int ii = i; ii < i + blocksize; ++ii) { // block iterators
    //         //   for (int jj = j; jj < j + blocksize; ++jj) {

    //             // for (int outer = 0; outer < lim; outer++) { // need this to run 3 times
    //             //   for (int finally = 0; finally < lim - dec; finally++) { // let this row repeat 3 times
    //             //     // int b_row = j + 1 + finally;
    //             //     for (int b_row = j + 1; b_row < j + blocksize - dec; b_row++) {
    //             //       for (int b_col = i + dec; b_col < i + blocksize - 1; b_col++) {

    //             //       temp = B[b_row][b_col];
    //             //       B[b_row][b_col] = B[b_row - 1][b_col + 1];
    //             //       B[b_row - 1][b_col + 1] = temp;
    //             //       printf("did something: \n");
    //             //       printM(M, N, B);

    //             //     }

    //             //     dec++;

    //             //    }
    //               // }
                  
                  
    //             // }
                
                

    //           // }
    //         }
          
            // // current correct version
            // for (int ii = i; ii < i + blocksize; ++ii) {
            //  for (int jj = j; jj < j + blocksize; ++jj) {
          
            //       B[jj][ii] = A[ii][jj];
            //       printf("EXCHANGE PERFORMED: (%d, %d) is now %d\n", jj, ii, A[ii][jj]);
            //       printM(M, N, B);  
            //   }
            // }
             
     
    

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

int isDiagonal(int i, int j) {
  return (
    (i < 8 && i >= 0) && (j < 8 && j >= 0) ||
    (i < 16 && i >= 8) && (j < 16 && j >= 8) ||
    (i < 24 && i >= 16) && (j < 24 && j >= 16) ||
    (i < 32 && i >= 24) && (j < 32 && j >= 24) ||
    (i < 40 && i >= 32) && (j < 40 && j >= 32) ||
    (i < 48 && i >= 40) && (j < 48 && j >= 40) ||
    (i < 56 && i >= 48) && (j < 56 && j >= 48) ||
    (i < 64 && i >= 56) && (j < 64 && j >= 56) 
  );
}

// work on something here...
void best_64(int M, int N, int A[N][M], int B[M][N]) {

 int blocksize = 4;
 int temp;
 int i;
 int j; 
 int ii;
 int jj;
 int ff = 0;
 int a_row; int b_row; int a_col; int b_col;
 // do the lower half in a zig zag pattern
 // need to iterate 2 * N times, then - 2, then - 2
  // need to start in upper left block, then down, then right, then down, then right....
  // and a has to be opposite.
  a_row = 0; a_col = 0; b_row = 0; b_col = 0; i = N; j = M;
  for (int k = 1; k <= (N / 8); k++) {
    while (a_col < i) {
      // transpose this block
      for (ii = a_row; ii < a_row + blocksize; ii++){
        for (jj = a_col; jj < a_col + blocksize; jj++) {
          B[jj][ii] = A[ii][jj];
        }
      }

      if (ff == 0) {
        a_col += blocksize;
        ff = 1;
      } else {  
        a_row += blocksize;
        ff = 0;
      }
    }
    ff = 0;
    a_row = 0;
    a_col = 8 * k;
  }

  ff = 0;
  a_row = 4; a_col = 0; b_row = 0; b_col = 0; i = N; j = M;
  for (int k = 1; k <= (N / 8); k++) {
    while (a_row < i) {
     
      // transpose this block
      for (ii = a_row; ii < a_row + blocksize; ii++){
        for (jj = a_col; jj < a_col + blocksize; jj++) {
          B[jj][ii] = A[ii][jj];
        }
      }

      if (ff == 0) {
        a_row += blocksize;
        ff = 1;
      } else {  
        a_col += blocksize;
        ff = 0;
      }
    }
    ff = 0;
    a_row = 8 * k;
    a_col = 0;
  }
 
 

  printf("maybe along diag?\n");
  printM(M, N, B);


//  ii = 0;
//  for (int k = (2 * N); k >= 2; k -= 2) {
//   jj = 0; 
//   for (int b_walker = 0; b_walker < k; b_walker) {
//     B[ii]
//   }
//   ii += 2;
//   for (int j = 0; j < M; j += blocksize) {
//     for (int i = 0; i < N; i += blocksize) {
 
//     for (ii = i; ii < i + blocksize; ii++){
//       for (jj = i; jj < i + blocksize; jj++) {
//         B[jj][ii] = A[ii][jj];
//       }
//     }
//   }
//  } 
//  }
// for (int j = 0; j < M; j += blocksize) {
//   for (int i = 0; i < N; i += blocksize) {
 
//     for (ii = i; ii < i + blocksize; ii++){
//       for (jj = i; jj < i + blocksize; jj++) {
//         B[jj][ii] = A[ii][jj];
//       }
//     }
//   }
//  } 

// // STEP 1, place the contents of Diags of A into right shifted diags of B
// for (i = 0; i < N - blocksize; i += blocksize) {
//   for (ii = i; ii < i + blocksize; ii++){
//     for (jj = i; jj < i + blocksize; jj++) {
//       B[ii][jj + blocksize] = A[ii][jj];
//     }
//   }
// } 



// blocksize = 4;
// // STEP 2, somewhat normal transpose for all cases except diagonal
  
//    for (i = 0; i < N; i += blocksize) { // row block increaser
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
//                   temp = A[ii][ii]; // load up A
//                   for (int jj = j; jj < j + blocksize; jj++) {
//                       if (jj != ii) {
//                           B[jj][ii] = A[ii][jj];
//                       }
//                   }
//                   B[ii][ii] = temp; // load up b for next it
//               }
//             }
//             // printf("diag at (%d, %d)\n", i, j);
           
//           } else {
//             // for (int ii = i; ii < i + blocksize; ii++) {
//             //       for (int jj = j; jj < j + blocksize; jj++) {
//             //           B[jj][ii] = A[ii][jj];
//             //       }
//             //   }
//             if (j < M - 8) { // NOT the last row
              

//               for (int ii = i; ii < i + blocksize; ii++) {
//                   for (int jj = j; jj < j + blocksize; jj++) {
//                       B[jj][ii] = A[ii][jj];
//                   }
//               }
//             }
//           }
       
//       }
  
//     }



// printM(M, N, B);

  //   // this goes left -> right
  // for (i = 0; i < N; i += blocksize) { // row block increaser
  //     for (j = 0; j < M; j += blocksize) { // col block increaser
          
  //         if (i == j) {
  //             for (int ii = i; ii < i + blocksize; ii++) {
  //                 temp = A[ii][ii]; // load up A
  //                 for (int jj = j; jj < j + blocksize; jj++) {
  //                     if (jj != ii) {
  //                         B[jj][ii] = A[ii][jj];
  //                     }
  //                 }
  //                 B[ii][ii] = temp; // load up b for next it
  //             }
  //         } else {
  //             for (int ii = i; ii < i + blocksize; ii++) {
  //                 for (int jj = j; jj < j + blocksize; jj++) {
  //                     B[jj][ii] = A[ii][jj];
  //                 }
  //             }
  //         }
          
  //     }
  
  //   }

   
}


void init(int M, int N, int A[N][M]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      A[i][j] = j;
    }
  }
}

void initZ(int M, int N, int A[N][M]) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      A[i][j] = 0;
    }
  }
}

void normal(int M, int N, int A[N][M], int B[M][N]) {
  int i, j, tmp;
  int diag;

    for (i = N - 1; i >= 0; i--) { // backwards rows
      diag = A[i][i];
      for (j = M - 1; j >= 0; j--) { // backward cols
          if (i != j) {
            tmp = A[i][j];
            B[j][i] = tmp;
          }
          

          printf("\n");
          printM(M, N, B);
      }
      B[i][i] = diag;
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
  const int size = 32;
  int M = size; int N = size;
  int A[size][size];
  int B[size][size];

  init(M, N, A);
  initZ(M, N, B);
  
  // best_32(M, N, A, B);
  best_64(M, N, A, B);
   printf("done:\n");
  printM(M, N, B);
  
  printf("A\n");
  printM(M, N, A);
  

 
  return 0;
}
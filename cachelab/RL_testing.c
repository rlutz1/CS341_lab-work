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

  // DIAGS -----------------------------------------------------------
 int blocksize = 8; int i; int j; int ii; int jj; int diag_row_fill; int diag_col_fill; 

//  // fill in each block of 8 by 8 to 8x8 to the right
//   for (i = 0; i < N - blocksize; i += blocksize) {
//     for (ii = i; ii < i + blocksize; ii++){
//       for (jj = i; jj < i + blocksize; jj++) {
//         B[ii][jj + blocksize] = A[ii][jj];
//       }
//     }
//   } 


//   // avoid final problem by putting last diag to left
//   for (ii = N - blocksize; ii < N; ii++) {
//     for (jj = N - blocksize; jj < N; jj++) {
//       B[ii][jj - blocksize] = A[ii][jj];
//     }
//   }

  

  // now, let's fill the diagonals FROM B
  // we're gonna fill as a square shape starting in top left
  // blocksize = 4;
  // for (i = 0, j = 0; i < N; i += 8, j += 8) {

  //   if (i < M - 8) { // NOT the last row  
  //     for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
  //       for (int jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = B[ii][jj + 8];
  //       }
  //     }

  //     for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
  //       for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
  //           B[jj][ii] = B[ii][jj + 8];
  //       }
  //     }

  //       for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
  //       for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
  //           B[jj][ii] = B[ii][jj + 8];
  //       }
  //     }

  //     for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
  //       for (int jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = B[ii][jj + 8];
  //       }
  //     }
  //   } else {

  //     for (int ii = i; ii < i + blocksize; ii++) { // fill the upper left square
  //       for (int jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = B[ii][jj - 8];
  //       }
  //     }

  //     for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
  //       for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
  //           B[jj][ii] = B[ii][jj - 8];
  //       }
  //     }

  //     for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
  //       for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
  //           B[jj][ii] = B[ii][jj - 8];
  //       }
  //     }

  //     for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
  //       for (int jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = B[ii][jj - 8];
  //       }
  //     }
  //   }
  // }


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

    // for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
    //   for (int jj = j + blocksize, k = 0; jj < j + (2 * blocksize); jj++, k++) {
    //       B[jj][ii] = B[ii][48 + k];
    //   }
    // }


    for (int ii = i, diag_row_fill = 0; ii < i + blocksize; ii++, diag_row_fill++) { // fill the bottom left square
      for (int jj = j + blocksize, diag_col_fill = 60; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }
    //  for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
    //   for (int jj = j; jj < j + blocksize; jj++) {
    //       B[jj][ii] = B[ii][jj + 8];
    //   }
    // }

    for (int ii = i + blocksize, diag_row_fill = 0; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the bottom right square
      for (int jj = j + blocksize, diag_col_fill = 52; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }

    //   for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
    //   for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
    //       B[jj][ii] = B[ii][jj + 8];
    //   }
    // }
   
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

    // for (int ii = i; ii < i + blocksize; ii++) { // fill the upper right square
    //   for (int jj = j + blocksize, k = 0; jj < j + (2 * blocksize); jj++, k++) {
    //       B[jj][ii] = B[ii][48 + k];
    //   }
    // }


    for (int ii = i, diag_row_fill = 60; ii < i + blocksize; ii++, diag_row_fill++) { // fill the bottom left square
      for (int jj = j + blocksize, diag_col_fill = 4; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }
    //  for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
    //   for (int jj = j; jj < j + blocksize; jj++) {
    //       B[jj][ii] = B[ii][jj + 8];
    //   }
    // }

    for (int ii = i + blocksize, diag_row_fill = 60; ii < i + (2 * blocksize); ii++, diag_row_fill++) { // fill the bottom right square
      for (int jj = j + blocksize, diag_col_fill = 12; jj < j + (2 * blocksize); jj++, diag_col_fill++) {
          B[jj][ii] = B[diag_row_fill][diag_col_fill];
      }
    }

    //   for (int ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
    //   for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
    //       B[jj][ii] = B[ii][jj + 8];
    //   }
    // }
   
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

  

  // // now, next step is that we need to do these little L's under the diagonal
  // i = 0; j = 8; ff = 0;
  // for (i = 0, j = 8; i < M; i += 8, j += 8) {
  //   for (ii = i; ii < i + blocksize; ii++) { // fill the upper left square
  //       for (jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = A[ii][jj];
  //       }
  //   }

  //   for (ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom left square
  //       for (jj = j; jj < j + blocksize; jj++) {
  //           B[jj][ii] = A[ii][jj];
  //       }
  //   }

  //   for (ii = i + blocksize; ii < i + (2 * blocksize); ii++) { // fill the bottom right square
  //       for (int jj = j + blocksize; jj < j + (2 * blocksize); jj++) {
  //           B[jj][ii] = A[ii][jj];
  //       }
  //   }
  // }

  // printf("filled in little ls\n");
  // printM(M,N,B);


  // // now, to fill in the remainders
  // i = 0; j = 12; ff = 0;
  // for (int k = 1; k <= 7; k++) { // do it one less time than num diagonals
  //   while (jj < N) { // while col traveller is still on the matrix

  //   }
  // }

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
// //  int j; 
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
//         B[ii][jj - 16] = A[ii][jj];
//       }
//    }
//  printf("asdlkashdnlsam\n");
//   printM(M, N, B);

//    // do the lower half in a zig zag pattern
//  // need to iterate 2 * N times, then - 2, then - 2
//   // need to start in upper left block, then down, then right, then down, then right....
//   // and a has to be opposite.
//   ff = 0;
//   a_row = 0; a_col = 0; i = N; 

//   while (a_col < i) {
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//           for (jj = a_col; jj < a_col + blocksize; jj++) {
//               // B[jj][ii] = A[ii][jj];
//               if ((a_row >= 56 && a_row < 64) && (a_col >= 56 && a_col < 64)) {
//                   B[jj][ii] = B[ii][jj - 16];
//                   printf("filling something %d %d\n", ii, jj);
//                   printM(M, N, B);
//               } else {
//                     B[jj][ii] = B[ii][jj + 8];
//               }
              
//           }

//           printf("filling something %d %d\n", ii, jj);
//           printM(M, N, B);
//           // printf("filling something\n");
//           // printM(M, N, B);
//       }

//       if (ff == 0) {
//           a_col += blocksize;
//           ff = 1;
//       } else {  
//           a_row += blocksize;
//           ff = 0;
//       }
//   }

//   printf("asdlkashdnlsam\n");
//   printM(M, N, B);

//   ff = 0;
//   a_row = 0; a_col = 0; i = N; 
  
//     while (a_row < i) {
     
//         if (a_row == a_col) {
          
//           if (ff == 0) {
//             a_row += blocksize;
//             ff = 1;
//           } else {  
//             a_col += blocksize;
//             ff = 0;
//           }
//           continue;
//         }
//       // transpose this block
//       for (ii = a_row; ii < a_row + blocksize; ii++){
//         for (jj = a_col; jj < a_col + blocksize; jj++) {
//          // B[jj][ii] = A[ii][jj];
//             if ((a_row >= 56 && a_row < 64) && (a_col >= 56 && a_col < 64)) {
//                 B[jj][ii] = B[ii][jj - 16];
//             } else {
//                 B[jj][ii] = B[ii][jj + 8];
//             }
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
 

//   // handle diagonals SPECIFICALLY
// //   for (i = 0; i < N; i += blocksize) { // row block increaser
// //     for (j = 0; j < M; j += blocksize) { // col block increaser
        
// //       if (isDiagonal(i, j)) { // for now since buggy

// //         if (j < M - 8) { // NOT the last row
// //           for (int ii = i; ii < i + blocksize; ii++) { // reach ahead and pull from B!
// //               for (int jj = j; jj < j + blocksize; jj++) {
// //                   B[jj][ii] = B[ii][jj + 8];
// //               }
// //           }
// //         } else { // mitigate damage on final diag block
// //             for (int ii = i; ii < i + blocksize; ii++) { // reach ahead and pull from B!
// //               for (int jj = j; jj < j + blocksize; jj++) {
// //                   B[jj][ii] = B[ii][jj - 8];
// //               }
// //             }
// //           }
// //         }
// //       } 
// //     }
  






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

// for this we are receiving a 67 x 61
// and the OUTPUT will be 61 x 67
void best_67_61(int M, int N, int A[N][M], int B[M][N]) {

  int row_A = 0;
  int col_A = 0; int col_trav; int counter; int offset;
  int blocksize = 8;

  for (offset = 0; offset < M; offset += blocksize) {
    row_A = 0;
    col_A = offset;
    while (row_A < N) { // less than 67, don't reach past last
      for (col_trav = col_A, counter = 0; counter < blocksize; col_trav = (col_trav + 1) % M, counter++) {
        printf("loading (%d, %d)\n", row_A, col_trav);
        B[col_trav][row_A] = A[row_A][col_trav];
      }

      col_A = (col_A + 3) % M;
      row_A++;
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
  int M = 61; int N = 67;
  int A[N][M];
  int B[M][N];

  init(M, N, A);
  initZ(M, N, B);
  
  // best_32(M, N, A, B);
  best_67_61(M, N, A, B);
   printf("done:\n");
  printM(M, N, B);
  
  // printf("A\n");
  // printM(M, N, A);
  

 
  return 0;
}
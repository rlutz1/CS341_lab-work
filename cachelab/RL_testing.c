#include <stdio.h>

// best for 32 SO FAR
// running with 344 misses, which is super close
void best_32(int M, int N, int A[N][M], int B[M][N]) {
    int blocksize = 8;

   for (int i = 0; i < N; i += blocksize) { // row block increaser
    for (int j = 0; j < M; j += blocksize) { // col block increaser
        
        for (int ii = i; ii < i + blocksize; ii++) {
            for (int jj = j; jj < j + blocksize; jj++) {
                B[jj][ii] = A[ii][jj];
            }
        }
    }
  }
}

// work on something here...
void best_64(int M, int N, int A[N][M], int B[M][N]) {
   int blocksize = 8;
  //  int test = 32;
   
   for (int i = 0; i < N; i += blocksize) { // row block increaser
    for (int j = 0; j < M; j += blocksize) { // col block increaser
        
        for (int ii = i; ii < i + blocksize; ii++) {
            for (int jj = j; jj < j + blocksize; jj++) {
                B[jj][ii] = A[ii][jj];
            }
        }
    }
  }
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


    for (int i = 0; i < N; i++) {
      B[i][i] = A[i][i];
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
  const int size = 8;
  int M = size; int N = size;
  int A[size][size];
  int B[size][size];

  init(M, N, A);

  blocker_2(M, N, A, B);

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d, ", B[i][j]);
    }
    printf("\n");
  }

  printf("done");
  return 0;
}
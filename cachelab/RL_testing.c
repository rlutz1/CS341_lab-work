#include <stdio.h>

void blocker(int M, int N, int A[N][M], int B[M][N]) {

    int last_size = N; // we are assuming M == N here!
    int row;
    int col; 
    int test = 0;

    for (int log = (N / 2); log > 0; log /= 2) { // N = 8, iterate 4, 2, 1 -- 3 times
        printf("log is %d\n", log);

        for (int jumper = 0; jumper < N - 1; jumper += last_size) { // size to jump for next block
            printf("jumper is %d\n", jumper);

            // issue, need to stay on that row for longer
          
          
          for (int grab_per_row = 0; grab_per_row < 4; grab_per_row++) { // needs to iterate 4 times
              row = log + jumper + (test % log); // + col _ shifter


              for(int col_shifter = 0; col_shifter < log; col_shifter++) { // 0 -> 4, 0 -> 2, 0 -> 1
                  printf("iterator inner is %d\n", col_shifter);
                  
                  col = jumper + col_shifter;
                  
                  printf("ACCESSING B: (%d, %d)\n", row, col);
                  B[row][col] = A[col][row];
                  B[col][row] = A[row][col];

              }
            test++; 
          }
          test = 0;
        }
        last_size /= 2;
    }

} // end method

int main() {

  int M = 8;
  int N = 8;
  int A[8][8] = { {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8},
                  {1, 2, 3, 4, 5, 6, 7, 8}
                };

  int B[8][8] = {0};
  blocker(M, N, A, B);

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d, ", B[i][j]);
    }
    printf("\n");
  }
  return 0;
}
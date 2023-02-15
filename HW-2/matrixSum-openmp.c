/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

double start_time, end_time;

int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];

/* a struct that holds a number with 
its row and col indexes in matrix */
struct num_info {
  int num;
  int row;
  int col;
};

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, total=0;
  
  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

  omp_set_num_threads(numWorkers);

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
    //printf("[ ");
	  for (j = 0; j < size; j++) {
      matrix[i][j] = rand()%100000;
      //printf(" %d", matrix[i][j]);
	  }
	  //printf(" ]\n");
  }

/* Used to find the max and min numbers with their indexes in matrix*/
#pragma omp declare reduction(maximum : struct num_info : omp_out = (omp_in.num> omp_out.num) ? omp_in : omp_out) \
initializer(omp_priv = {INT_MIN, 0, 0})

#pragma omp declare reduction(minimum : struct num_info : omp_out = (omp_in.num< omp_out.num) ? omp_in : omp_out) \
initializer(omp_priv = {INT_MAX, 0, 0})

    struct num_info maxNum = {INT_MIN, 0, 0};
    struct num_info minNum = {INT_MAX, 0, 0};

    start_time = omp_get_wtime();
  #pragma omp parallel for reduction (+:total) reduction (maximum:maxNum) reduction(minimum:minNum) private(j)
    for (i = 0; i < size; i++)
      for (j = 0; j < size; j++){
        total += matrix[i][j];
        int currentNum = matrix[i][j];
        if(currentNum < minNum.num) {
          minNum.num = currentNum;
          minNum.row = i+1;
          minNum.col = j+1;
        }
        if(currentNum > maxNum.num) {
          maxNum.num = currentNum;
          maxNum.row = i+1;
          maxNum.col = j+1;
        }
      }

  // implicit barrier
    end_time = omp_get_wtime();

    printf("the total is %d\n", total);
    printf("Min number in matrix is %d, located at [%d][%d]\n", minNum.num, minNum.row, minNum.col);
    printf("Max number in matrix is %d, located at [%d][%d]\n", maxNum.num, maxNum.row, maxNum.col);
    printf("it took %g seconds\n", end_time - start_time);
}


/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/

#include <omp.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 1000  /* maximum matrix size */
#define MAXWORKERS 8   /* maximum number of workers */

double start_time, end_time;

int numWorkers;
int size; 
int matrix[MAXSIZE][MAXSIZE];
void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j, total=0;
  int minNumRow, minNumCol = 0;
  int maxNumRow, maxNumCol = 0;

  int minNum = INT_MAX;
  int maxNum = INT_MIN;

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
      matrix[i][j] = rand()%999;
      	  //printf(" %d", matrix[i][j]);
	  }
	  	  //printf(" ]\n");
  }

  //printf("minNum is equal to %d at start\n", minNum);
  //printf("maxNum is equal to %d at start\n", maxNum);

  start_time = omp_get_wtime();
#pragma omp parallel for reduction (+:total) private(j)
  for (i = 0; i < size; i++)
    for (j = 0; j < size; j++){
      total += matrix[i][j];
      int currentNum = matrix[i][j];
#pragma omp critical
      if(currentNum < minNum) {
        minNum = currentNum;
        minNumRow = i+1;
        minNumCol = j+1;
      }
      if(currentNum > maxNum) {
        maxNum = currentNum;
        maxNumRow = i+1;
        minNumCol = j+1;
      }
    }

// implicit barrier

  end_time = omp_get_wtime();

  printf("the total is %d\n", total);
  printf("Min number in matrix is %d, located at [%d][%d]\n", minNum, minNumRow, minNumCol);
  printf("Max number in matrix is %d, located at [%d][%d]\n", maxNum, maxNumRow, maxNumCol);
  printf("it took %g seconds\n", end_time - start_time);

}


/* matrix summation using pthreads

   features: uses a barrier; the Worker[0] computes
             the total sum from partial sums computed by Workers
             and prints the total sum to the standard output

   usage under Linux:
     gcc matrixSum.c -lpthread
     a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int numArrived = 0;       /* number who have arrived */

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

//min_max[] holds max, min and pos for every strip in matrix. 
//First three columns hold minNum, minNumRow, minNumCol.
//Last three columns hold maxNum, maxNumRow, maxNumCol. 

int min_max[MAXWORKERS][6];

/* a reusable counter barrier */
void Barrier() {
  pthread_mutex_lock(&barrier);
  numArrived++;
  if (numArrived == numWorkers) {
    numArrived = 0;
    pthread_cond_broadcast(&go);
  } else
    pthread_cond_wait(&go, &barrier);
  pthread_mutex_unlock(&barrier);
}

/* timer */
double read_timer() {
    static bool initialized = false;
    static struct timeval start;
    struct timeval end;
    if( !initialized )
    {
        gettimeofday( &start, NULL );
        initialized = true;
    }
    gettimeofday( &end, NULL );
    return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void *Worker(void *);

//Find maxNum in a given matrix strip and store in min_max[]
void findMaxInStrip(int first, int last, int l) {
  int max, maxRow, maxCol = 0;
  int i, j;
  for(i = first; i < last; i++) {
    for(j = 0; j < size; j++) {
      if(matrix[i][j] > max) {
        max = matrix[i][j];
        maxRow = i+1;
        maxCol = j+1;
      }
    }
  }
  min_max[l][3] = max;
  min_max[l][4] = maxRow;
  min_max[l][5] = maxCol;
}

//FInd globalMax and print result to terminal. 
void findGlobalMax(void) {
  int max, maxRow, maxCol = 0;
  int i;
  for(i = 0; i < MAXWORKERS; i++) {
    if(min_max[i][3] > max) {
      max = min_max[i][3];
      maxRow = min_max[i][4];
      maxCol = min_max[i][5];
    } 
  }
  printf("Max number in matrix is %d, located at [%d][%d]\n", max, maxRow, maxCol);
}

//Find minNum in a given matrix strip and store in min_max[]
void findMinInStrip(int first, int last, int l) {
  int minRow, minCol = 0;
  int min = matrix[first][0];
  int i, j;
  for(i = first; i < last; i++) {
    for(j = 0; j < size; j++) {
      if(matrix[i][j] < min) {
        min = matrix[i][j];
        minRow = i+1;
        minCol = j+1;
      }
    }
  }
  min_max[l][0] = min;
  min_max[l][1] = minRow;
  min_max[l][2] = minCol;
}

//Find globalMin and print to terminal
void findGlobalMin(void) {
  int min = min_max[0][0];
  int minRow = min_max[0][1];
  int minCol = min_max[0][2];
  int i;
  for(i = 0; i < MAXWORKERS; i++) {
    if(min_max[i][0] < min) {
      min = min_max[i][0];
      minRow = min_max[i][1];
      minCol = min_max[i][2];
    } 
  }
  printf("Min number in matrix is %d, located at [%d][%d]\n", min, minRow, minCol);
}

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
  int i, j;
  long l; /* use long in case of a 64-bit system */
  pthread_attr_t attr;
  pthread_t workerid[MAXWORKERS];

  /* set global thread attributes */
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

  /* initialize mutex and condition variable */
  pthread_mutex_init(&barrier, NULL);
  pthread_cond_init(&go, NULL);

  /* read command line args if any */
  size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
  numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
  if (size > MAXSIZE) size = MAXSIZE;
  if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
  stripSize = size/numWorkers;
  //printf("The strip is: %d", stripSize);  
  printf("\n"); 

  /* initialize the matrix */
  for (i = 0; i < size; i++) {
	  for (j = 0; j < size; j++) {
          matrix[i][j] = rand()%999;
	  }
  }

  /* print the matrix */
  #ifdef DEBUG
  for (i = 0; i < size; i++) {
	  printf("[ ");
	  for (j = 0; j < size; j++) {
	    printf(" %d", matrix[i][j]);
	  }
	  printf(" ]\n");
  }
  #endif



  /* do the parallel work: create the workers */
  
  start_time = read_timer();
  for (l = 0; l < numWorkers; l++)
    pthread_create(&workerid[l], &attr, Worker, (void *) l);
  pthread_exit(NULL);
}

/* Each worker sums the values in one strip of the matrix.
   After a barrier, worker(0) computes and prints the total */
void *Worker(void *arg) {
  long myid = (long) arg;
  int total, i, j, first, last;

#ifdef DEBUG
  printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
#endif

  /* determine first and last rows of my strip */
  first = myid*stripSize;
  last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

  findMinInStrip(first, last, myid);
  findMaxInStrip(first, last, myid);

  /* sum values in my strip */

  total = 0;
  for (i = first; i <= last; i++)
    for (j = 0; j < size; j++)
      total += matrix[i][j];
  sums[myid] = total;

  

  Barrier();
  if (myid == 0) {
    findGlobalMin();
    findGlobalMax();
    total = 0;
    for (i = 0; i < numWorkers; i++)
      total += sums[i];
    /* get end time */
    end_time = read_timer();
    /* print results */
    printf("The total is %d\n", total);
    printf("The execution time is %g sec\n", end_time - start_time);

    //Uncomment to print the matrix that contains min and max in every strip of matrix

    /* for (i = 0; i < MAXWORKERS; i++) {
      printf("[ ");
      for (j = 0; j < 6; j++) {
        printf(" %d", min_max[i][j]);
      }
      printf(" ]\n");
    }  */
  }

  
}
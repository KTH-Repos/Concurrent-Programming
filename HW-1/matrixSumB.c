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

int maxNum, maxNumRow, maxNumCol;

int minNumRow, minNumCol, minNum;

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

void findMaxForGlobal(int first, int last) {
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
  pthread_mutex_lock(&barrier);
  if(max > maxNum) {
      maxNum = max;
      maxNumRow = maxRow;
      maxNumCol = maxCol;
      pthread_mutex_unlock(&barrier);
  }
  else{
    pthread_mutex_unlock(&barrier);
  }
}

void findMinForGlobal(int first, int last) {
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
  pthread_mutex_lock(&barrier);
  if(min < minNum) {
    minNum = min;
    minNumRow = minRow;
    minNumCol = minCol;
    pthread_mutex_unlock(&barrier);
  }
  else{
    pthread_mutex_unlock(&barrier);
  }
}

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

  minNum = matrix[MAXSIZE-1][MAXSIZE-1];
  minNumCol = MAXSIZE-1;
  minNumRow = MAXSIZE-1;

  maxNum = 0;
  maxNumCol = 0;
  maxNumRow = 0;

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

  for (l = 0; l < numWorkers; l++)
     pthread_join(workerid[l], NULL);

  printf("This is printed by main thread\n");
  printf("Min number in matrix is %d, located at [%d][%d]\n", minNum, minNumRow, minNumCol);
  printf("Max number in matrix is %d, located at [%d][%d]\n", maxNum, maxNumRow, maxNumCol);
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

    findMinForGlobal(first, last);
    findMaxForGlobal(first, last);

    total = 0;
    for (i = first; i <= last; i++)
        for (j = 0; j < size; j++)
            total += matrix[i][j];
    sums[myid] = total;

    Barrier();
    if (myid == 0) {
    total = 0;
    for (i = 0; i < numWorkers; i++)
      total += sums[i];
    /* get end time */
    end_time = read_timer();
    /* print results */
    printf("The total is %d\n", total);
    printf("The execution time is %g sec\n", end_time - start_time);
  }

  
}
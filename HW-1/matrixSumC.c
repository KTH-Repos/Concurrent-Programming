#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#define MAXSIZE 10000 /* maximum matrix size */
#define MAXWORKERS 10   /* maximum number of workers */
#define MODULONUM 9999    // defined a constant for number used to generate numbers in matrix     !!!

pthread_mutex_t barrier;  /* mutex lock for the barrier */
pthread_cond_t go;        /* condition variable for leaving */
int numWorkers;           /* number of workers */ 
int numArrived = 0;       /* number who have arrived */

double start_time, end_time; /* start and end times */
int size, stripSize;  /* assume size is multiple of numWorkers */
int sums[MAXWORKERS]; /* partial sums */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

/* Global variables for max,min and their positions in matrix */    //!!!
int maxNumRow, maxNumCol = 0;   

int minNumRow, minNumCol = 0;

int minNum = INT_MAX;
int maxNum = INT_MIN;

//int globalSum;        //deleted globalSum    !!!

int bag_of_tasks_counter = 0;

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

void findMaxForGlobal(int first) {
  int max, maxRow, maxCol = 0;
  int i, j;
  i = first;
  for(j = 0; j < size; j++) {
    if(matrix[i][j] > max) {
      max = matrix[i][j];
      maxRow = i+1;
      maxCol = j+1;
    }
  }
  
  pthread_mutex_lock(&barrier);
  if(max > maxNum && max < MODULONUM) {       //added a checker so that numers that arent supposed to be generated end up being the max   !!!
      maxNum = max;
      maxNumRow = maxRow;
      maxNumCol = maxCol;
      pthread_mutex_unlock(&barrier);
  }
  else{
    pthread_mutex_unlock(&barrier);
  }
}

void findMinForGlobal(int first) {
  int minRow, minCol = 0;
  int min = 100;
  int i, j;
  i = first;
  for(j = 0; j < size; j++) {
    if(matrix[i][j] < min) {
      min = matrix[i][j];
      minRow = i+1;
      minCol = j+1;
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
          matrix[i][j] = rand()%MODULONUM;
	  }
  }

  /* print the matrix */
  #ifdef DEBUG
  for (i = size-2; i < size; i++) {
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
    int currentRow;
    long myid = (long) arg;
    int total, i, j, first, last;

    #ifdef DEBUG
    printf("worker %ld (pthread id %ld) has started\n", myid, pthread_self());
    #endif

    while(currentRow < size) {

      pthread_mutex_lock(&barrier);
      currentRow = bag_of_tasks_counter;
      bag_of_tasks_counter++;
      pthread_mutex_unlock(&barrier);

      if(currentRow >= size)
        break;

      total = 0;
      for (j = 0; j < size; j++)
          total += matrix[currentRow][j];
      sums[myid] += total;          //every row is summed up and added to partial array for sums  !!!

      findMinForGlobal(currentRow);
      findMaxForGlobal(currentRow);

    }
    //Removed this part from the while loop     !!!
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

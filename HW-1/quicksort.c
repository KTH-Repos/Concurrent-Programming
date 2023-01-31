#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 100  /* maximum list size */


pthread_mutex_t lock;
int list[MAXSIZE];

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

//swap function 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void initList(void) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        list[i] = rand()%100;
    }
}

void printList(void) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        printf("%d ", list[i]);
    }
}

int main(int argc, char*argv []) {
    initList();
    printList();

}


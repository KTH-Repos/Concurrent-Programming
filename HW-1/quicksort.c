#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 20  /* maximum list size */
#define MAXWORKERS 10   /* maximum number of workers */



pthread_mutex_t lock;
int activeWorkers = 0;
int list[MAXSIZE];
struct sublist_data {
    int start;
    int end;
};

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

int partition(int start, int end, int pivot) {
    
    int pivotElem =list[pivot];
    swap(&list[pivot], &list[end]);
    int startIndex = start;

    for(int i = start; i < end; i++) {
        if(list[i] <= pivotElem){
            swap(&list[i], &list[startIndex]);
            startIndex++;
        }
    }
    swap(&list[startIndex], &list[end]);
    return startIndex;

}

void *quickSort_parallel(void *sublist_data) {
    struct sublist_data *data = (struct sublist_data*) sublist_data;
    int start = data->start;
    int end = data->end;
    quickSort(start, end);
} 

void quickSort(int start, int end) {

   /*  pthread_mutex_lock(&lock);
    if(activeWorkers <= MAXWORKERS) {
        activeWorkers++;
        pthread_mutex_unlock(&lock); */
        if(start < end) {
            int pivot = start + (end-start)/2;
            int pivotPos = partition(start, end, pivot);

            pthread_t thread;
            struct sublist_data sublist;
            sublist.start = start;
            sublist.end = pivot;
            pthread_create(&thread, NULL, quickSort_parallel, (void *) &sublist);
            printf("(pthread id %ld) has started\n", pthread_self());
            quickSort(pivotPos+1, end);

            pthread_join(thread, NULL);
        }
    /* }
    else {
        pthread_mutex_unlock(&lock);
    } */
    

   
}

void initList(void) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        list[i] = rand()%999;
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
    printf("\n");
    pthread_t starterthread;
    struct sublist_data mainList;
    mainList.start = 0;
    mainList.end = MAXSIZE-1;

    pthread_t starter;
    pthread_create(&starter, NULL, quickSort_parallel, (void *) &mainList);
    pthread_join(starter, NULL);
    printList();



}


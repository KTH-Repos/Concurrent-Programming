#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 100  /* maximum list size */


pthread_mutex_t lock;
int list[MAXSIZE];
struct sublist_data {
    int start;
    int end;
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

//swap function 
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int start, int end, int pivot) {
    
    int pivotElem =*list[pivot];
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

void *quickSort(void *sublist_data) {
    struct sublist_data *data = (struct sublist_data*) sublist_data;
    int start = data->start;
    int end = data->end;

    int pivot = start + (end-start)/2;
    int pivot = partition(start, end, pivot);

    if(start < pivot-1){
        pthread_t left_thread;
        struct sublist_data left_subarray;
        left_subarray.start = start;
        left_subarray.end = pivot-1;
        pthread_create(&left_thread, NULL, quickSort, (void *) &left_subarray);
        pthread_join(left_thread, NULL);
    }

    if(pivot+1 < end) {
        pthread_t right_thread;
        struct sublist_data right_subarray;
        right_subarray.start = pivot+1;
        right_subarray.end = end;
        pthread_create(&right_thread, NULL, quickSort, (void *) &right_subarray);
        pthread_join(right_thread, NULL);
    }

    pthread_exit(NULL);
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


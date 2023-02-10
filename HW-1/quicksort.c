#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 100   /* maximum list size */
#define MAXWORKERS 10   /* maximum number of workers */

pthread_mutex_t lock;
int activeWorkers = 0;
int list[MAXSIZE];
struct sublist_data {
    int *sublist;
    int start;
    int end;
};

void task_coordinator(int *sublist, int start, int end);

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

int partition(int *sublist, int start, int end, int pivot) {
    
    int pivotElem =sublist[pivot];
    swap(&sublist[pivot], &sublist[end]);
    int startIndex = start;

    for(int i = start; i < end; i++) {
        if(sublist[i] < pivotElem){
            swap(&sublist[i], &sublist[startIndex]);
            startIndex++;
        }
    }
    swap(&sublist[startIndex], &sublist[end]);
    return startIndex;
}

void quickSort_sequential(int *sublist, int start, int end) {
    if(start < end) {
        int pivot = start + (end-start)/2;
        int pivotPos = partition(sublist, start, end, pivot);

        quickSort_sequential(sublist, start, pivotPos-1);
        quickSort_sequential(sublist, pivotPos+1, end);
    }
}

void *quickSort_parallel(void *sublist_data) {
    struct sublist_data *data = (struct sublist_data*) sublist_data;
    task_coordinator(data->sublist, data->start, data->end);
} 

void task_coordinator(int *sublist, int start, int end) {

    if(start < end) {
        if(end-start>100) {

            int pivot = start + (end-start)/2;
            int pivotPos = partition(sublist, start, end, pivot);

            pthread_mutex_lock(&lock);
            if(activeWorkers < MAXWORKERS) {
                activeWorkers++;
                pthread_mutex_unlock(&lock); 

                pthread_t thread;
                struct sublist_data sublist_sub = {sublist, start, pivotPos-1};
                pthread_create(&thread, NULL, quickSort_parallel, (void *) &sublist_sub);
                
                printf("(pthread id %ld) has started\n", pthread_self());
                task_coordinator(sublist, pivotPos+1, end);

                pthread_join(thread, NULL);
                pthread_mutex_lock(&lock);
                activeWorkers--;
                pthread_mutex_unlock(&lock); 


            }
            else {
                pthread_mutex_unlock(&lock);
                quickSort_sequential(sublist, start, end);
            } 
        }
        else{
            //printf("sublist is too small to be sorted by new thread\n");
            quickSort_sequential(sublist, start, end);
        }
    }
}

void initList(int *list) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        list[i] = rand()%9999;
    }
}

void printList(int *list) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        printf("%d ", list[i]);
    }
}

int main(int argc, char*argv []) {
    initList(list);

    printList(list);

    double starttime = read_timer();

    task_coordinator(list, 0, MAXSIZE-1);

    double finishtime = read_timer();

    printf("\n");
    printList(list);


    printf("Time taken by algo : %g sec", finishtime-starttime);

    return 0;

}
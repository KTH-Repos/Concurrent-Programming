/*The number of threads and size of array to be sorted is hard-coded in the program and 
cannot be modified with input from terminal before execution  */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 100000  /* maximum matrix size */
#define MAXWORKERS 2   /* maximum number of workers */

double start_time, end_time;

int list[MAXSIZE];      //list that will contain numbers to be sorted

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

void quickSort_parallel(int *sublist, int start, int end) {
    if(start < end) {

        int pivot = start + (end-start)/2;
        int pivotPos = partition(sublist, start, end, pivot);

        #pragma omp task
        {
            quickSort_parallel(sublist, start, pivotPos-1);
        }

        #pragma omp task
        {
            quickSort_parallel(sublist, pivotPos+1, end);
        }

    }
}

void initList(int *list) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        list[i] = rand()%1000;
    }
}

void printList(int *list) {
    int i;
    for(i = 0; i < MAXSIZE; i++) {
        printf("%d ", list[i]);
    }
}

int main(int argc, char*argv[]) {
    initList(list);

    //printList(list);

    //printf("\n\n");

    omp_set_num_threads(MAXWORKERS);

    
        start_time = omp_get_wtime();

        #pragma omp parallel 
        {
            #pragma omp single
            quickSort_parallel(list, 0, MAXSIZE-1);
        }

        end_time = omp_get_wtime();

        //printList(list);

        //printf("\n\n");

        printf("it took %g seconds\n", end_time - start_time);

}
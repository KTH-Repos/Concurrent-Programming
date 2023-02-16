#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define CONSUMERS 3
#define WORMS 5

//n baby birds(CONSUMERS), one parent bird(PRODUCER), W worms

pthread_t babybirds[CONSUMERS];
pthread_t parent;
int availableWorms;

sem_t full, empty;
sem_t mutexC;

void *producing(void *arg) {
    while(true) {
        sem_wait(&empty);   //P(empty)
        availableWorms = WORMS;
        printf("parent just filled in the dish with %d worms\n\n", availableWorms);
        sem_post(&full);
    }
}

void *consuming(void *arg) {
    long myID = (long) arg;
    while(true) {
        sem_wait(&full);
        sem_wait(&mutexC);   //lock critical section
        if(availableWorms > 0) {
            availableWorms--;
            printf("babybird %ld ate one worm and there are %d worms left\n\n", myID, availableWorms);
            sem_post(&mutexC);      //unlock critical section
            sem_post(&full);
            //printf("babybird %ld is going to sleep now\n\n", myID);
            sleep(2);
        }
        else {
            printf("babybird %ld found empty dish, waiting for parent ...\n\n", myID);
            sem_post(&mutexC);
            sem_post(&empty);
        }
    }
}

int main(int argc, char*argv[]) {

    int numbabybirds = CONSUMERS;
    availableWorms = WORMS;
    sem_init(&empty, true, 0);
    sem_init(&full, true, 1);
    sem_init(&mutexC, false, 1);    //used as mutex lock

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&parent, &attr, producing, NULL);
    long l;
    for (l = 0; l < numbabybirds; l++)
        pthread_create(&babybirds[l], &attr, consuming, (void *)l);

    pthread_join(parent, NULL);
    for(l = 0; l < numbabybirds; l++)
        pthread_join(babybirds[l], NULL);
    

}


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define CONSUMERS 3
#define WORMS 5

//N baby birds(CONSUMERS), one parent bird(PRODUCER), W worms

pthread_t babybirds[CONSUMERS];     //N baby birds
pthread_t parent;
int availableWorms;                 //symbolizes dish with worms, accessed by babybirds and parent

sem_t full, empty;                  //semaphores for mutex lock and conditional synchronization 


/* Function executed by parent */
void *producing(void *arg) {
    while(true) {
        sem_wait(&empty);   //P(empty) - dish is empty and parent must fill in with worms.
        availableWorms = WORMS;
        printf("parent just filled in the dish with %d worms\n\n", availableWorms);
        sem_post(&full);    //V(full) - dish is full and babybirds can start eating
    }
}

/* Function executed by babybirds */
void *consuming(void *arg) {
    long myID = (long) arg;     //id for babybird
    while(true) {
        sem_wait(&full);    //P(full) - babybird wants to eat worm. If no worms avaiable then wait until parent bird fills dish and signal it.
        if(availableWorms > 0) {
            availableWorms--;   //babybird takes one worm. This is critical section guarded by the semaphore full
            printf("babybird %ld ate one worm and there are %d worms left\n\n", myID, availableWorms);
            sem_post(&full); //V(full) - babybird signals for others to come in and take worm
            sleep(2); //babybird sleeps for two secs 
        }
        else {
            printf("babybird %ld found empty dish, waiting for parent ...\n\n", myID);
            sem_post(&empty);   //babybird found no worm to take and signals to parent to fill in dish
        }
    }
}

int main(int argc, char*argv[]) {

    int numbabybirds = CONSUMERS;       //number of babybirds
    availableWorms = WORMS;             //number of worms is set to W from start
    sem_init(&empty, true, 0);          //semaphore used by babybirds to signal parent
    sem_init(&full, true, 1);           //semahore used by babybirds as mutexlock & signaling mechanism between babybirds and from parentbird to waiting babybirds

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


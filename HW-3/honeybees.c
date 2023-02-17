#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define PRODUCERS 3
#define HONEYPOT_CAPACITY 5

//N honeybees, one hungry bear, honeypot with capacity H

pthread_t bees[PRODUCERS];      //N honeybees
pthread_t bear;
int honeypot_portions;          //symbolized honeypot, accessed by honeybees and bear

sem_t full, empty;              //semaphores for mutex lock and conditional synchronization 


/* FUnction executed by honeybees */
void *producing(void *arg) {
    long myID = (long) arg;
    while(true) {
        sem_wait(&empty);       //P(empty) - honeypot is empty and bees must fill it in until its full
        honeypot_portions++;    //fill in honeypot with one portion
        if(honeypot_portions == HONEYPOT_CAPACITY) {
            printf("bee %ld made honey pot full, about to wake up bear ...\n\n", myID);
            sem_post(&full);    //bee who filled the last portion signals the bear
        }
        else {
            printf("bee %ld filled with one portion, currently %d portions in honey pot\n\n", myID, honeypot_portions);
            sem_post(&empty);   //V(empty) - bee signals the others to fill in honeypot with portion until its full
            sleep(2);           //bee sleeps for two secs
        }
    }
}

/* Function executed by bear */
void *consuming(void *arg) {
    while(true) {
        sem_wait(&full);        //P(full) - honeypot is full and bear wakes up to eat 
        honeypot_portions = 0;
        printf("the bear ate all honey portions, 0 portions left\n\n");
        sem_post(&empty);       //V(empty) - honeypot is empty and bees can start filling in with portions
    }
}


int main(int argc, char*argv[]) {

    int numbees = PRODUCERS;        //number of bees
    honeypot_portions = 0;          //number of portions in honeypot is 0 from start   
    sem_init(&empty, true, 1);      //semaphore used by bees as mutexlock & signaling mechanism between bees and from bear to producing bees
    sem_init(&full, true, 0);       //semaphore used by bees to signal bear

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&bear, &attr, consuming, NULL);
    long l;
    for(l = 0; l < numbees; l++)
        pthread_create(&bees[l], &attr, producing, (void *)l);

    pthread_join(bear, NULL);
    for(l = 0; l < numbees; l++) {
        pthread_join(bees[l], NULL);
    }


}
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define PRODUCERS 3
#define HONEYPOT_CAPACITY 5

//n honeybees, one hungry bear, honeypot with capacity H

pthread_t bees[PRODUCERS];
pthread_t bear;
int honeypot_portions;

sem_t full, empty;
sem_t mutexC;

void *producing(void *arg) {
    long myID = (long) arg;
    while(true) {
        sem_wait(&empty);
        sem_wait(&mutexC);
        honeypot_portions++;
        if(honeypot_portions == HONEYPOT_CAPACITY) {
            printf("bee %ld made honey pot full, about to wake up bear ...\n\n", myID);
            sem_post(&mutexC);
            sem_post(&full);
        }
        else {
            printf("bee %ld filled one portion, currently %d portions in honey pot\n\n", myID, honeypot_portions);
            sem_post(&mutexC);
            sem_post(&empty);
            sleep(3);
        }
    }
}

void *consuming(void *arg) {
    while(true) {
        sem_wait(&full);
        honeypot_portions = 0;
        printf("the bear ate all honey portions, 0 portions left\n\n");
        sem_post(&empty);
    }
}


int main(int argc, char*argv[]) {

    int numbees = PRODUCERS;
    honeypot_portions = 0;      

    sem_init(&empty, true, 1);
    sem_init(&full, true, 0);
    sem_init(&mutexC, true, 1);     //used as mutex lock

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
//
//  project2_part1.cpp
//  
//
//  Created by Laura Adams on 4/21/16.
//
//

//#include "project2_part1.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
//#include <omp.h>
#include <unistd.h>

#define BUFFER_SIZE 10

sem_t mutex;
sem_t full;
sem_t empty;

bool stop = false;

int buffer[BUFFER_SIZE];

void *insert_item(void *value); //insert a random integer (1-1000) to the buffer
void *remove_item(void *value); // remove one integer from the buffer

int in = 0;
int out = 9;


int main(int argc, char *argv[]){
    
    if (argc != 4) {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }
    
    if (atoi(argv[1]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }
    
    if (atoi(argv[2]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }
    
    if (atoi(argv[3]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }
    
    
    //argv[1] = time to execute in sec
    //argv[2] = # of producer threads
    //argv[3] = # of consumer threads
    
    int timeToRun = atoi(argv[1]);
    int numProducers = atoi(argv[2]);
    int numConsumers = atoi(argv[3]);
    
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1);
    
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    /*int producerThreadNum[numProducers];
    int consumerThreadNum[numConsumers];
    
    for (int i = 0; i < numProducers; i ++){
        producerThreadNum[i] = i;
    }
    
    for (int i = 0; i < numConsumers; i++){
        consumerThreadNum[i] = i;
    }*/
    
    
    //start time
    
    int i, j, a, b;
    for (i = 0; i < BUFFER_SIZE; i++){
        buffer[i] = -1;
    }
    
    for (i = 0; i < numProducers; i++){
        a = i;
        pthread_create(&tid, &attr, insert_item, (void*)(intptr_t)a);

    }
    
    for ( j = 0; j < numConsumers; j++){
        b = j;
        pthread_create(&tid, &attr, remove_item, (void*)(intptr_t)b);
    }
    
    sleep(timeToRun);
    stop = true;
    return 0;
    //end time
    
}


void *insert_item(void *value){ //insert a random integer (1-1000) to the buffer
     in = (intptr_t)value;
     in = in%10;
    int producerID = (intptr_t)value;
    //value is starting point of array
    
    int next_produced;
    srand((unsigned)time(NULL));
    
    do{
        
        
        next_produced = rand() % 1000;
        //produce next item to next_produced
        
        sem_wait(&empty);
        sem_wait(&mutex);
        
        //add next produced to buffer
        if (buffer[in] == -1){
            buffer[in] = next_produced;
            printf(" At index %i, Producer ID %i: %i\n", in, producerID, buffer[in]);
        }
        in = (in +1) % BUFFER_SIZE;
        
        sem_post(&mutex);
        sem_post(&full);
        
    } while(!stop);
    
}

void *remove_item(void *value){ // remove one integer from the buffer
    out = (intptr_t)value;
    out = out%10;
    int next_consumed = -1;
    int consumerID = (intptr_t)value;
    //value starting point of thread
    do{
        sem_wait(&full);
        sem_wait(&mutex);
        
        // remove item from buffer to next_consumed
        if (buffer[out] != -1){
            printf("        At index %i, Consumer ID %i: %i\n", out, consumerID, buffer[out]);
            buffer[out] = next_consumed;
        }
        
        
        sem_post(&mutex);
        sem_post(&empty);
        
        // comsume item in next consumed
        out = (out+1) % BUFFER_SIZE;
        
    } while(!stop);
    
}

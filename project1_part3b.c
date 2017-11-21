//
//  project1_part3b.c
//  using pthreads
//
//  Created by Laura Adams on 3/20/16.
//
//using pthread to find the amount of 99 in array

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREAD 8

#define SIZE 100000
//static pthread_mutex_t mutexcount = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;
int count99 = 0;
void *counter( void *t);

int myArray[SIZE];
struct param{
    int work;
    int ID;
    int upper;
    int lower;
};

int main(){
    int i;
    double start_time, end_time, speedUP, serialTime, parallel2, parallel4, parallel8;
    
      /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init (&count_threshold_cv, NULL);
    
    //initialize random number genertor
    srand((unsigned)time(NULL));
    
    //initialize the array using ramdom numbers
    for (i = 0; i < SIZE; i++){
        myArray[i] = rand() % 100;
    }
    
    //serial code
    start_time = omp_get_wtime();
    for (i = 0; i < SIZE; i++ ){
        if(myArray[i] == 99){
            count99++;
        }
    }
    end_time = omp_get_wtime();
    serialTime = end_time - start_time;
    printf ("The serial code indicates that there are %d 99s in the array \n\n", count99);
    printf ("The serial code used %f seconds to complete the execution. \n\n", serialTime);
    
    //using pthreads
    unsigned int usedThreads;
    struct param data[NUM_THREAD];
    pthread_t threads[NUM_THREAD];
    pthread_attr_t attr;
    
    //for portability
    pthread_attr_init(&attr);
    
    printf("Using 2 threads\n\n");
    //spawn 2 threads
    usedThreads = 2;
    for(i = 0; i < usedThreads; i++){
        data[i].ID = i;
        data[i].work = SIZE / usedThreads;
        data[i].upper = ((data[i].ID)+1)*(data[i].work)-1;
        data[i].lower = (data[i].ID)*(data[i].work);
    }
    
    start_time = omp_get_wtime();
    for (i = 0; i < usedThreads; ++i){
        pthread_create(&threads[i], &attr, counter, (void*) &data[i]);
    }

    end_time = omp_get_wtime();
    parallel2 = end_time - start_time;
    speedUP = serialTime / parallel2;
    printf ("The parallel code (2 pthreads) indicates that there are %d 99s in the array \n\n", count99);
    printf ("The parallel code (2 pthreads) used %f seconds to complete the execution. \n\n", parallel2);
    printf ("Speed up time: %f \n", speedUP);
    
    printf("Using 4 threads\n\n");
    //spawn 4 threads
    usedThreads = 4;
    count99 = 0;
    for(i = 0; i < usedThreads; i++){
        data[i].ID = i;
        data[i].work = SIZE / usedThreads;
        data[i].upper = ((data[i].ID)+1)*(data[i].work)-1;
        data[i].lower = (data[i].ID)*(data[i].work);
    }
    
    start_time = omp_get_wtime();
    for (i = 0; i < usedThreads; ++i){
        pthread_create(&threads[i], &attr, counter, (void*) &data[i]);
    }
    
    for( i = 0; i <usedThreads; ++i){
        pthread_join(threads[i],NULL);
    }
    
    end_time = omp_get_wtime();
    parallel4 = end_time - start_time;
    speedUP = serialTime / parallel4;
    printf ("The parallel code (4 pthreads) indicates that there are %d 99s in the array \n\n", count99);
    printf ("The parallel code (4 pthreads) used %f seconds to complete the execution. \n\n", parallel4);
    printf ("Speed up time: %f \n", speedUP);
    
    printf("Using 8 threads\n\n");
    //spawn 8 threads
    usedThreads = 8;
    count99 = 0;
    for(i = 0; i < usedThreads; i++){
        data[i].ID = i;
        data[i].work = SIZE / usedThreads;
        data[i].upper = ((data[i].ID)+1)*(data[i].work)-1;
        data[i].lower = (data[i].ID)*(data[i].work);
    }
    
    start_time = omp_get_wtime();
    for (i = 0; i < usedThreads; ++i){
        pthread_create(&threads[i], &attr, counter, (void*) &data[i]);
    }
    
    end_time = omp_get_wtime();
    parallel8 = end_time - start_time;
    speedUP = serialTime / parallel8;
    printf ("The parallel code (8 pthreads) indicates that there are %d 99s in the array \n\n", count99);
    printf ("The parallel code (8 pthreads) used %f seconds to complete the execution. \n\n", parallel8);
    printf ("Speed up time: %f \n", speedUP);
    
    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit (NULL);
    
}


void *counter( void *t){
    int i;
    struct param *temp = (struct param*)t;
    for (i = temp->lower; i <= temp->upper; i++){
        if (myArray[i] == 99){
            pthread_mutex_lock(&count_mutex);
            count99++;        //need a barrier to prevent data racing
            pthread_mutex_unlock(&count_mutex);
        }
    }
    pthread_exit(0);
}
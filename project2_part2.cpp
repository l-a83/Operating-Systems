//
//  project2_part2.cpp
//  
//
//  Created by Laura Adams on 4/25/16.
//
//

//#include "project2_part2.hpp"


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

#define MAX_CHAIRS 3

sem_t mutex;
sem_t student_TAmutex;
sem_t student_student_mutex;

pthread_mutex_t ta_and_Stu_mutex;
pthread_mutex_t stu_mutex;

int studentcount;
int numChairs;
bool helping = false;
bool stop = false;

void *taFunction(void *value);
void *studentfunction(void *value);

int main(int argc, char *argv[]){
    
     //printf("initialize\n");
    
    if (argc != 3) {
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
    
   
    int timeToRun = static_cast<double>(atoi(argv[1]));
    int numStudents = atoi(argv[2]);
    
    sem_init(&mutex, 0, 0);
    sem_init(&student_TAmutex, 0, 1);
    sem_init(&student_student_mutex, 0, 1);

    pthread_t taThread;
    pthread_t studentThreads[numStudents];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    studentcount = 0;
    numChairs = 0;
    helping = false;
    
    int a;
    int i;
    
    //printf("create student threads\n");
    for (i = 0; i < numStudents; i++){
        a = i+1;
        pthread_create(&studentThreads[i], &attr, studentfunction, (void *)(intptr_t)a);
    }
    
    //printf("create ta thread\n");
    pthread_create(&taThread, &attr, taFunction, NULL);

    
    
    
    sleep(timeToRun);
    stop = true;
    return 0;

}

void *taFunction(void *value){
    //printf("enter ta thread\n");
    do{
        
        if(numChairs > 0){
            sem_wait(&student_TAmutex);
            printf("TA: chair count before help: %i\n", numChairs);
            helping = true;
            numChairs--;
            printf("TA: changed chair count after help: %i\n", numChairs);
            printf("TA is done helping\n");
            helping = false;
            sem_post(&student_TAmutex);
            
        }
        
        else if ( numChairs == 0 && !helping){
            printf("TA is sleeping\n");
            //helping = false;
        }
        
        
        
    }while (!stop);
    
}

void *studentfunction(void *value){
    int ID = (intptr_t)(value);
    printf("enter student %i thread\n", ID);
    do{
    pthread_mutex_lock(&stu_mutex);
    //sem_wait(&student_student_mutex);
    
    //studentcount++;
    //sem_post(&student_student_mutex);
    
    if (numChairs == 0 && helping == false) {
        printf("Student %i asked for help\n", ID);
        sem_wait(&student_TAmutex);
        numChairs++;
        printf("Student %i: changed chair count: %i\n", ID, numChairs);
        sem_post(&student_TAmutex);
        //to wait up TA ?
    }
    
    else if (numChairs >= 0 && numChairs < MAX_CHAIRS && helping == true){
        printf("Student %i must wait outside\n", ID);
        sem_wait(&student_TAmutex);
        numChairs++;
        printf("Student %i: changed chair count: %i\n", ID, numChairs);
        sem_post(&student_TAmutex);
        sleep(0.001);
    }
    
    else if (numChairs >= 3 /*&& studentcount >= MAX_CHAIRS*/){
        printf("stduent %i went back to programming\n", ID);
        printf("chair count: %i\n", numChairs);
        sleep(0.001);
        //studentcount--;
    }
    
    //sem_post(&student_student_mutex);
      pthread_mutex_unlock(&stu_mutex);
    }while (!stop);
    
}


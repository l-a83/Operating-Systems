//
//  project1_1a.c
//  
//
//  Created by Laura Adams on 3/15/16.
//
//using pthread to create a fibSequence

#include "project1_part1a.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//int userFibNum;

#define N 50

void *fibCreate(void *num);
int fibArray[N];

int main(int argc, char *argv[]){
    
    //int *fibArray = new int[];
    
    int size;
    
    pthread_t tid;
    pthread_attr_t attr;
    
    printf("Fibonacci Sequence \n");
    
    if (argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n");
                return -1;
    }
                
    if (atoi(argv[1]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }
    
    //once user input is valid, create child process to create sequence
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, fibCreate, (void*)argv[1] );
    
    pthread_join(tid, NULL);
    
    size = atoi(argv[1]);
    
    if (size == 0){
        printf( " %i element : %i\n" , size , fibArray[size]);
        printf(" the %i fibonacci number is %i\n", size, fibArray[size]);
    }
        
    for( int i = 0; i < size; i++){
        printf( " %i element : %i\n" , i , fibArray[i]);
    }
    
    if (size > 0)
        printf(" the %i fibonacci number is %i\n", size, fibArray[size-1]);
    
}

void *fibCreate(void *num){
    int i, sequenceLimit = atoi((char*)num);
    
        fibArray[0] = 0;
    
    if (sequenceLimit > 0){
        fibArray[1] = 1;
    
    
        for (i = 2; i < sequenceLimit; i++){
            fibArray[i] = fibArray[i-1] + fibArray[i-2];
        }
    }
    
    pthread_exit(0);
}



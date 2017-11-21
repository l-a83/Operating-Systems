//
//  project1_part3a.c
//
//
//  Created by Laura Adams on 3/20/16.
//
//using opm to find the amount of 99 in array

//#include "project1_part3a.h"
#include "omp.h"
//#include <iostream>
#include "stdio.h"
#include "stdlib.h"
//#include <ctime>
#include "time.h"
//using namespace std;

int main(){
    
    int count = 0, i;
    int const SIZE = 100000;
    int myArray[SIZE];
    double start_time, end_time, serialTime, parallel2Time, parallel4Time, parallel8Time, speedUP;
    
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
            count++;
        }
    }
    end_time = omp_get_wtime();
    
    serialTime = end_time - start_time;
    
    printf ("The serial code indicates that there are %d 99s in the array \n\n", count);
    printf ("The serial code used %f seconds to complete the execution. \n\n", serialTime);
    
    //parallel with 2 threads
    omp_set_num_threads(2);
    printf ("num of threads %d \n",omp_get_num_threads());  //?
    
    count = 0;
    start_time = omp_get_wtime();
    
#pragma omp parallel for num_threads(2) private(i) shared(myArray) reduction (+:count)
    for (i = 0; i < SIZE; i++ ){
        if(myArray[i] == 99){
            count++;
        }
        //if ( i == 0 || i == SIZE-1)
           // printf("executed by thread %d\n", omp_get_thread_num());
        //printf("count %i\n", count);
    }
    
    end_time = omp_get_wtime();
    parallel2Time = end_time - start_time;
    speedUP = serialTime / parallel2Time;
    
    printf ("The parallel code (2 threads) indicates that there are %d 99s in the array \n\n", count);
    printf ("The parallel code (2 threads) used %f seconds to complete the execution. \n\n", parallel2Time);
    printf ("Speed up time: %0.9f \n", speedUP);
    
    //parallel with 4 threads
    omp_set_num_threads(4);
    printf ("num of threads %d \n",omp_get_num_threads());
    count = 0;
    start_time = omp_get_wtime();
    //num_threads(4)
#pragma omp parallel for num_threads(4) private(i) shared(myArray) reduction (+:count)
    for (i = 0; i < SIZE; i++ ){
        if(myArray[i] == 99){
            count++;
        }
        //if ( i == 0 || i == SIZE-1 || i == (SIZE/4)+5 || i == 2*(SIZE/4)+5)
            //printf("executed by thread %d\n", omp_get_thread_num());
        //printf("count %i\n", count);
    }
    end_time = omp_get_wtime();
    parallel4Time = end_time - start_time;
    speedUP = serialTime / parallel4Time;
    
    printf ("The parallel code (4 threads) indicates that there are %d 99s in the array \n\n", count);
    printf ("The parallel code (4 threads) used %f seconds to complete the execution. \n\n", parallel4Time);
    printf ("Speed up time: %0.9f \n", speedUP);
    
    //parallel with 8 threads
    omp_set_num_threads(8);
    printf ("num of threads %d \n",omp_get_num_threads());
    count = 0;
    start_time = omp_get_wtime();
    
#pragma omp parallel for num_threads(8) private(i) shared(myArray) reduction (+:count)
    for (i = 0; i < SIZE; i++ ){
        if(myArray[i] == 99){
            count++;
        }
        //printf("executed by thread %d\n", omp_get_thread_num());
        //printf("count %i\n", count);
    }
    end_time = omp_get_wtime();
    parallel8Time = end_time - start_time;
    speedUP = serialTime / parallel8Time;
    
    printf ("The parallel code (8 threads) indicates that there are %d 99s in the array \n\n", count);
    printf ("The parallel code (8 threads) used %f seconds to complete the execution. \n\n", parallel8Time);
    printf ("Speed up time: %0.9f \n", speedUP);
    
    
}
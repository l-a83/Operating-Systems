//
//  project1_part1b.c
//
//
//  Created by Laura Adams on 3/20/16.
//
//using Win32 to create a FibSequence

//#include "project1_part1b.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>

//int userFibNum;

#define N 50

DWORD WINAPI fibCreate(LPVOID num);
DWORD fibArray[N];

int main(int argc, char *argv[]){

    //int *fibArray = new int[];

    int size, i;
    DWORD ThreadId;
    HANDLE ThreadHandle;



    if (argc != 2) {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    if (atoi(argv[1]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    size =  atoi(argv[1]);
    //once user input is valid, create child process to create sequence
    ThreadHandle = CreateThread(NULL,0,fibCreate, &size, 0, &ThreadId);

    if (ThreadHandle != NULL){
        WaitForSingleObject(ThreadHandle, INFINITE);
    }
    CloseHandle(ThreadHandle);


    printf("Fibonacci Sequence \n");

    if (size == 0){
        printf( " %i element : %i\n" , size , fibArray[size]);
        printf(" the %i fibonacci number is %i\n", size, fibArray[size]);
    }

    for( i = 0; i <= size; i++){
        printf( " %i element : %i\n" , i , fibArray[i]);
    }
    if(size > 0)
        printf("the %i fibonacci number is %i", size, fibArray[size-1]);

}

DWORD WINAPI fibCreate(LPVOID num){
    DWORD i, sequenceLimit = *(DWORD*)num;

        fibArray[0] = 0;
    DWORD fibnum2 = 1;
    if(sequenceLimit > fibnum2){
        fibArray[1] = 1;

        for (i = 2; i <= sequenceLimit; i++){
            fibArray[i] = fibArray[i-1] + fibArray[i-2];
        }
    }

    return 0;
}



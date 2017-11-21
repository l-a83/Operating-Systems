//
//  project1_part2a.c
//
//
//  Created by Laura Adams on 3/21/16.
//
//

//#include "project1_part2a.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

struct v{
    int i; //row
    int j; //column
};

void *matrixElement (void *param);

#define M 3
#define K 2
#define N 3
#define NUM_THREADS M*N
int A[M][K] = { {1,4}, {2,5}, {3,6} };
int B[K][N] = { {8,7,6}, {5,4,3} };
int C[M][N];


int main()

{
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int i, j;
    
    //create worker threads using strategy below
    for( i = 0; i<M; i++){
        for( j = 0; j<N; j++){
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            pthread_create(&tid, NULL, matrixElement, data);
        }
    }

    pthread_join(tid, NULL);

    
    printf("Matrix C: \n");
    for ( i = 0; i < M; i++){
        for ( j = 0; j < N; j++){
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    return 0;
    
}

void *matrixElement (void *param){
    struct v *data = (struct v*)param;
    int row = data->i;
    int column = data->j;
    int j, element = 0;
    for ( j = 0; j < K; j++){
        element = element + (A[row][j]*B[j][column]);
    }
    C[row][column] = element;
    pthread_exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

int MAX;

// Structure to pass arguments to threads
typedef struct {
    int startRow;
    int endRow;
} ThreadArgs;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  ThreadArgs* threadArgs = (ThreadArgs*) args;
  for (int i = threadArgs->startRow; i < threadArgs->endRow; i++) {
      for (int j = 0; j < MAX; j++) {
          matSumResult[i][j] = matA[i][j] + matB[i][j];
      }
  }
  free(threadArgs);
  pthread_exit(NULL);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  ThreadArgs* threadArgs = (ThreadArgs*) args;
  for (int i = threadArgs->startRow; i < threadArgs->endRow; i++) {
      for (int j = 0; j < MAX; j++) {
          matDiffResult[i][j] = matA[i][j] - matB[i][j];
      }
  }
  free(threadArgs);
  pthread_exit(NULL);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  ThreadArgs* threadArgs = (ThreadArgs*) args;
  for (int i = threadArgs->startRow; i < threadArgs->endRow; i++) {
      for (int j = 0; j < MAX; j++) {
          matProductResult[i][j] = 0;
          for (int k = 0; k < MAX; k++) {
              matProductResult[i][j] += matA[i][k] * matB[k][j];
          }
      }
  }
  free(threadArgs);
  pthread_exit(NULL);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?

int main() {
    srand(time(0));

    // Fill matrices with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // Print initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // Create pthread_t objects
    pthread_t threads[THREAD_COUNT];

    // Compute Sum
    for (int i = 0; i < THREAD_COUNT; i++) {
        ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
        args->startRow = i * (MAX / THREAD_COUNT);
        args->endRow = (i + 1) * (MAX / THREAD_COUNT);
        pthread_create(&threads[i], NULL, computeSum, args);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Compute Difference
    for (int i = 0; i < THREAD_COUNT; i++) {
        ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
        args->startRow = i * (MAX / THREAD_COUNT);
        args->endRow = (i + 1) * (MAX / THREAD_COUNT);
        pthread_create(&threads[i], NULL, computeDiff, args);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Compute Product
    for (int i = 0; i < THREAD_COUNT; i++) {
        ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs));
        args->startRow = i * (MAX / THREAD_COUNT);
        args->endRow = (i + 1) * (MAX / THREAD_COUNT);
        pthread_create(&threads[i], NULL, computeProduct, args);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
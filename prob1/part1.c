#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define THREAD_NUM 8

sem_t semEmpty;
sem_t semFull;

sem_t mutexBuffer;

char buffer[10];
int count = 0;
int items_produced = 0;

int buffer_length;
int number_producer_threads;
int number_consumer_threads;
int number_items;

void* producer(void* tid) {
    while (1) {
        sem_wait(&semEmpty);
        sem_wait(&mutexBuffer);  
        if(items_produced == number_items) {
            sem_post(&mutexBuffer);
            sem_post(&semFull);
            printf("thread %d exiting produce\n", tid);
            pthread_exit(0);
        }
        char x = 'X';
        printf("p:<%d>, item: %c, at %d\n", (long)tid, x, count);

        buffer[count] = x;
        items_produced++;
        count++;
        sem_post(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* tid) {
    while (1) {
        sem_wait(&semFull);
        sem_wait(&mutexBuffer);
        if(count > 0) {
            char y = buffer[count - 1];
            count--;
            printf("c:<%d>, item: %c, at %d\n", (long)tid, y, count);
        }
        if(items_produced == number_items) {
            sem_post(&mutexBuffer);
            sem_post(&semEmpty);
            printf("thread %d exiting consume\n", tid);
            pthread_exit(0);
        }
        sem_post(&mutexBuffer);
        sem_post(&semEmpty);
    }
}

int main(int argc, char* argv[]) {

    buffer_length = atoi(argv[2]);
    number_producer_threads = atoi(argv[4]);
    number_consumer_threads = atoi(argv[6]);
    number_items = atoi(argv[8]);

    int total_threads = number_consumer_threads + number_producer_threads;

    printf("%d\n", buffer_length);
    printf("%d\n", number_producer_threads);
    printf("%d\n", number_consumer_threads);
    printf("%d\n", number_items);

    sem_init(&semEmpty, 0, buffer_length);
    sem_init(&semFull, 0, 0);
    sem_init(&mutexBuffer, 0, 1);


    pthread_t threads[total_threads];
    
    for (int i = 0; i < total_threads; i++) {
        pthread_t thread;
        threads[i] =thread;
    }
    for(int i = 0; i < total_threads; i++) {
        if(i < number_consumer_threads) {
            if (pthread_create(&threads[i], NULL, &consumer, (void *) i) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&threads[i], NULL, &producer, (void *) i) != 0) {
                perror("Failed to create thread");
            }          
        }
    }
    for (int i = 0; i < total_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    sem_destroy(&mutexBuffer);
    return 0;
}
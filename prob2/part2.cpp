#define N 10000000
#include <semaphore.h>
#include <iostream>

class Condition {
    public:
        int threads_blocked;
        sem_t sem;
};

Condition empty, full;
int count = 0;
int buffer_length, number_producer_threads, number_consumer_threads, number_items;
char buffer[N];


int count_threads(Condition cv) {

}

void wait(Condition cv) {
    
}

void signal(Condition cv) {

}

void mon_insert(char alpha) {
    while(count == N) {
        wait(full);
        buffer[count] = alpha;
        count = count + 1;
        signal(empty);
    }
}

char mon_remove() {
    char result;
    while(count == 0) {
        wait(empty);
        result = buffer[count];
        count = count - 1;
        signal(full);
        return result;
    }
}

void * producer() {
    char alpha;
    int r;
    while(1) {
        r = rand() % 26;
        alpha = 'a' + r;
        mon_insert(alpha);
    }
}

void * consumer() {
    char result;
    while(1) {
        result = mon_remove();
    }
}

void main (int argc, char* argv[]) {

    buffer_length = atoi(argv[2]);
    number_producer_threads = atoi(argv[4]);
    number_consumer_threads = atoi(argv[6]);
    number_items = atoi(argv[8]);

    int total_threads = number_consumer_threads + number_producer_threads;

    printf("%d\n", buffer_length);
    printf("%d\n", number_producer_threads);
    printf("%d\n", number_consumer_threads);
    printf("%d\n", number_items);
    sem_t emptySem;
    sem_init(&emptySem, 0, buffer_length);

    sem_t fullSem;
    sem_init(&fullSem, 0, 0);

    empty.sem = emptySem;
    empty.threads_blocked = 0;

    full.sem = fullSem;
    full.threads_blocked = 0;

    pthread_t threads[total_threads];

    srand(time(NULL));
    for(int i = 0; i < total_threads; i++) {
        if(i < number_consumer_threads) {
            if (pthread_create(&threads[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&threads[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }          
        }
    }
    for (int i = 0; i < total_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
}


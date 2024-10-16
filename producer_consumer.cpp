#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define THREAD_NUM 8
#define BUF_SZ 10

int buffer[BUF_SZ];
int count = 0;

pthread_mutex_t mutex;
sem_t sem_full; //if value <= 0, producer should not produce (buffer is full) and have to wait for consumer to consume some values in buffer 
sem_t sem_empty; //if value <= 0, consumer should not consume (buffer is empty) and have to wait for producer to producer some values in buffer

//produce a random number in buffer
void* producer(void* args) {
    while(1) {
        int x = rand() % 100;
        sleep(1);

        sem_wait(&sem_full); //if buffer is full, wait for consumer 
        if(count < BUF_SZ) {
            buffer[count] = x;
            count++;
        }
        else {
            printf("skipped %d\n", x);
            // sleep(1);
        }
        sem_post(&sem_empty); //increment value of sem_empty, so consumer can consume (buffer have some values)
    }
}

//consume a value in buffer
void* consumer(void* args) {
    while(1) {
        int y;
        sem_wait(&sem_empty); //if buffer is empty, wait for producer
        if(count > 0) {
            y = buffer[count - 1];
            --count;
        }
        else y = -1;
        sem_post(&sem_full); //increment value of sem_full, so producer can produce (a value is consumed, so buffer is not full)
        printf("Got %d\n", y);
        // sleep(1);
    }
}

int main() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_full, 0, BUF_SZ);
    sem_init(&sem_empty, 0, 0);

    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    for(int i = 0; i < THREAD_NUM; ++i) {
        if(i == 0) {
            if(pthread_create(&th[i], NULL, producer, NULL) != 0) {
                perror("create producer thread failed");
            }
        }
        else {
            if(pthread_create(&th[i], NULL, consumer, NULL) != 0) {
                perror("create consumer thread failed");
            }            
        }
    }

    for(int i = 0; i < THREAD_NUM; ++i) {
        if(pthread_join(th[i], NULL) != 0) {
            perror("join thread failed");
        }
    }
    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);
    pthread_mutex_destroy(&mutex);
    return 0;
}
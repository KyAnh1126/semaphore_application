#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define THREAD_NUM 16

sem_t semaphore;

int players = 4;

//perform login by client
//an application of semaphore is allow client to wait in the login queue
//and server can allow a number of clients (16 in this example) playing game at a same time
void* play_game(void* args) {
    long i = (long)args;
    printf("(%d) waiting in the login queue\n", i);
    sem_wait(&semaphore); //if value <= 0, player have to wait for others logged out
    printf("(%d) logged in\n", i);
    sleep(rand() % 5 + 1);
    printf("(%d) logged out\n", i);
    sem_post(&semaphore); //player finish, increment value of semaphore
}

int main() {
    pthread_t threads[THREAD_NUM];
    sem_init(&semaphore, 0, players);
    for(int i = 0; i < THREAD_NUM; ++i) {
        if(pthread_create(&threads[i], NULL, play_game, (void*)i) != 0) {
            perror("create thread failed");
        }
    }
    for(int i = 0; i < THREAD_NUM; ++i) {
        if(pthread_join(threads[i], NULL) != 0) {
            perror("join thread failed");
        }
    }
    sem_destroy(&semaphore);
    return 0;
}
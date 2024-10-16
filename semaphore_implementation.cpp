#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define THREAD_NUM 4

typedef struct {
    pthread_mutex_t mutex; //mutex to lock and unlock region of code
    pthread_cond_t condition; //condition allow thread to wait, or to send signal
    int value; //init value of semaphore, if value <= 0, thread have to wait for others, otherwise, thread do his work
} semaphore;

//init semaphore by init mutex, condition, and value
void semaphore_init(semaphore* sem, int initial_value) {
    pthread_mutex_init(&(sem->mutex), NULL);
    pthread_cond_init(&(sem->condition), NULL);
    sem->value = initial_value;
}

void semaphore_wait(semaphore* sem, long id) {
    pthread_mutex_lock(&(sem->mutex)); //lock mutex, value is accessed by only one thread at a time
    printf("thread %ld is locking (wait)\n", id);
    while (sem->value <= 0) {
        printf("thread %ld is waiting\n", id);
        pthread_cond_wait(&(sem->condition), &(sem->mutex)); //value <= 0 -> thread have to wait, unlock mutex temporarily, this thread cant function until receive signal & value > 0
    }
    sem->value--; //if thread funcion, decrement value
    printf("thread %ld decrement semaphore value\n", id);
    pthread_mutex_unlock(&(sem->mutex)); //unlock mutex
}

void semaphore_signal(semaphore* sem, long id) {
    pthread_mutex_lock(&(sem->mutex));
    printf("thread %ld is locking (signal)\n", id);
    sem->value++; //thread finish, increment value
    pthread_cond_signal(&(sem->condition)); //give signal to others, threads waiting by the condition variable can wake up and check value > 0 or not
    printf("thread %ld sent signal\n", id);
    pthread_mutex_unlock(&(sem->mutex));
}

//destroy semaphore by destroy mutex and condition
void semaphore_destroy(semaphore* sem) {
    pthread_mutex_destroy(&(sem->mutex));
    pthread_cond_destroy(&(sem->condition));
    sem->value = 0;
}

semaphore sem;

//example of wait and send signal of a thread
void* play_game(void* args) {
    long id = (long)args;
    semaphore_wait(&sem, id);
    printf("hello from thread %ld\n", id);
    sleep(1);
    printf("thread %ld slept\n", id);
    semaphore_signal(&sem, id);
}

int main() {
    pthread_t threads[THREAD_NUM];
    semaphore_init(&sem, 2);
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
    semaphore_destroy(&sem);
    return 0;
}
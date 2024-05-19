#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int shared_resource = 0;

#define NUM_ITERS 500000
#define NUM_THREADS 2000

void lock(int);
void unlock(int);

// for checking (1) whether it is deadlock (2) time measurement
// #define DEBUG 1

#ifdef DEBUG
// for time measurement
long long start_ms, end_ms;
struct timespec start, end;
#endif

int entering[NUM_THREADS];
int number[NUM_THREADS];

void lock(int i)
{
    entering[i] = 1;
    for(int j = 0; j < NUM_THREADS; ++j) {
        if(number[j] > number[i])
            number[i] = number[j];
    }
    number[i] += 1;
    entering[i] = 0;

    for(int j = 0; j < NUM_THREADS; ++j) {
        while(entering[j])
            usleep(1 * 1000);   // reduce busy waiting
        while((number[j] != 0)
            && ((number[j] < number[i]) || ((number[j] == number[i]) && (j < i))))
            usleep(1 * 1000);   // reduce busy waiting
    }
}

void unlock(int i)
{
    number[i] = 0;
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;
#ifdef DEBUG
    if(tid % 50 == 0)  printf("[start] thread %d\n", tid);
#endif

    lock(tid);

        for(int i = 0; i < NUM_ITERS; i++)    shared_resource++;

    unlock(tid);

#ifdef DEBUG
    if(tid % 50 == 0)  printf("[end] thread %d\n", tid);
#endif
    pthread_exit(NULL);
}

int main() {
    int n = NUM_THREADS;
    pthread_t threads[n];
    int tids[n];

#ifdef DEBUG
    clock_gettime(0, &start);
    start_ms = (start.tv_sec * 1000) + (start.tv_nsec / 1e6);
#endif
    for (int i = 0; i < n; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
#ifdef DEBUG
    clock_gettime(0, &end);
    end_ms = (end.tv_sec * 1000) + (end.tv_nsec / 1e6);
#endif

    printf("shared: %d\n", shared_resource);
#ifdef DEBUG
    printf("execution time: %lldms\n", (end_ms - start_ms));
#endif
    return 0;
}

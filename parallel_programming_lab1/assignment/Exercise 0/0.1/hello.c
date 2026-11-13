#include <stdio.h>
#include <pthread.h>

#ifndef NB_THREADS
#define NB_THREADS 10
#endif

void* hello(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < 3; i++)
    {
        printf("Hello world! (Thread %d)\n", id);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NB_THREADS];
    int thread_ids[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++)
    {
        thread_ids[i] = i;

        pthread_create(&threads[i], NULL, hello, &thread_ids[i]);
    }

    for (int i = 0; i < NB_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
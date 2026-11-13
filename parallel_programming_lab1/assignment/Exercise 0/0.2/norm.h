#ifndef NORM_H_
#define NORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stddef.h>


#ifndef LENGTH
#define LENGTH 1000000
#endif

#ifndef SEED
#define SEED 0
#endif

#ifndef SCALE
#define SCALE 10
#endif

#ifndef NB_THREADS
#define NB_THREADS 1
#endif

#if NB_THREADS > 1
#include <pthread.h>

typedef struct thread_arg {
  //Use this data structure to pass args

  size_t start;
  size_t end;
  double partial_sum;
} thread_arg;

void *dot(void *ptr);
#endif

double timediff(struct timespec *begin, struct timespec *end);

int main(int argc, char* argv[]);


#endif


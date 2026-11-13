#include "norm.h"

// This function computes a time span (double value)
// from two struct timespec pointers
double timediff(struct timespec *begin, struct timespec *end)
{
   double sec = 0.0, nsec = 0.0;
   if ((end->tv_nsec - begin->tv_nsec) < 0)
   {
      sec  = (double)(end->tv_sec  - begin->tv_sec  - 1);
      nsec = (double)(end->tv_nsec - begin->tv_nsec + 1000000000);
   } else
   {
      sec  = (double)(end->tv_sec  - begin->tv_sec );
      nsec = (double)(end->tv_nsec - begin->tv_nsec);
   }
   return sec + nsec / 1E9;
}

//Global variables
double v[LENGTH];
double norm = 0.;

#if NB_THREADS > 1
void *dot(void *ptr) {
    //Compute the norm of the vector via parallel computation.
    thread_arg *args = (thread_arg *)ptr;

    double sum = 0.0;
    for (size_t i = args->start; i < args->end; i++)
    {
      sum += v[i] * v[i];
    }
      args ->partial_sum = sum;

      return NULL;

    }
#endif

int main(int argc, char* argv[]) {

  //Check validity of constant parameters.
  if (sizeof(LENGTH) != sizeof(int) || LENGTH < 1) {
    printf("Invalid vector length.\n");
    return -1;
  }
  
  if (sizeof(SEED) != sizeof(int)) {
    printf("Invalid seed for random vectors.\n");
    return -1;
  }
  
  if (sizeof(SCALE) != sizeof(int) || SCALE < 1) {
    printf("Invalid scaling factor.\n");
    return -1;
  }
    
  if (sizeof(NB_THREADS) != sizeof(int)) {
    printf("Invalid number of threads.\n");
    return -1;
  }
  
  printf("Number of threads: %d.\n", NB_THREADS);

  //Initialize structures.
  srand(SEED);
  // Monitors general algorithm start and stop time.
  struct timespec begin, end;
  
  for (int i = 0; i < LENGTH; i++) v[i] = SCALE * rand() / (float)RAND_MAX;
  
  //Start the timer
  clock_gettime(CLOCK_MONOTONIC, &begin);
  
#if NB_THREADS < 2
  //Compute the vector norm (serial computation).
  double sum = 0.0;

  for(int i = 0; i < LENGTH; i++)
  {
    sum += v[i] * v[i];
  }
  norm = sqrt(sum);
#else
  //Start the threads.
  pthread_t threads[NB_THREADS];
  thread_arg args[NB_THREADS];

  for (int i = 0; i < NB_THREADS; i++)
  {
    args[i].start = (size_t)LENGTH * i / NB_THREADS;
    args[i].end = (size_t)LENGTH * (i + 1) / NB_THREADS;
    args[i].partial_sum = 0.0;

    int result = pthread_create(&threads[i], NULL, dot, &args[i]);
    if (result != 0) {
      fprintf(stderr, "Error creating thread %d\n", i);

      for (int j = 0; j < i; j++) {
        pthread_join(threads[j], NULL);
      }
      return -1;
    }
  }

  //Wait for the threads to finish.
  double total_sum = 0.0;

  for (int i = 0; i < NB_THREADS; i++)
  {
    pthread_join(threads[i], NULL);
    total_sum += args[i].partial_sum;
  }
  norm = sqrt(total_sum);
  
#endif
  
   //Stop the timer and print to screen the measured time.
   clock_gettime(CLOCK_MONOTONIC, &end);
   
   printf("The vector has norm: %f (computed in %f sec).\n", norm, timediff(&begin, &end));
 
  
  return 0;
    
}
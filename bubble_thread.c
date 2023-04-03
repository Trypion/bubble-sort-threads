#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// #define IMPRIME

typedef struct
{
  int **arrays;
  int array_size;
  int start;
  int end;
} SortParams;

void imprime(int **array, int num_arrays, int size)
{
  int i, j;
  for (i = 0; i < num_arrays; i++)
  {
    for (j = 0; j < size; j++)
    {
      printf("%d ", array[i][j]);
    }
    printf("\n");
  }
}

void bubble_sort(int **arrays, int start, int end, int size);
void *sort_thread(void *arg);

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Usage: %s <number of arrays> <array size> <number of threads>\n", argv[0]);
    return 1;
  }

  struct timeval t1, t2;
  double t_total;

  int num_arrays = atoi(argv[1]);
  int array_size = atoi(argv[2]);
  int num_threads = atoi(argv[3]);

  int **arrays = (int **)malloc(num_arrays * sizeof(int *));
  for (int i = 0; i < num_arrays; i++)
  {
    arrays[i] = (int *)malloc(array_size * sizeof(int));
    for (int j = 0; j < array_size; j++)
    {
      arrays[i][j] = rand() % 100;
    }
  }

#ifdef IMPRIME
  printf("Antes da ordenacao!\n");
  imprime(arrays, num_arrays, array_size);
  printf("\n");
#endif

  pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
  SortParams *params = (SortParams *)malloc(num_threads * sizeof(SortParams));

  int arrays_per_thread = num_arrays / num_threads;

  gettimeofday(&t1, NULL);
  for (int i = 0; i < num_threads; i++)
  {
    params[i].arrays = arrays;
    params[i].array_size = array_size;
    params[i].start = i * arrays_per_thread;
    params[i].end = (i == num_threads - 1) ? num_arrays : params[i].start + arrays_per_thread;
    pthread_create(&threads[i], NULL, sort_thread, &params[i]);
  }

  for (int i = 0; i < num_threads; i++)
  {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&t2, NULL);

#ifdef IMPRIME
  printf("Depois da ordenacao!\n");
  imprime(arrays, num_arrays, array_size);
  printf("\n");
#endif

  for (int i = 0; i < num_arrays; i++)
  {
    free(arrays[i]);
  }
  free(arrays);

  free(threads);
  free(params);

  t_total = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
  printf("%i; %f; %i; %i\n", num_arrays, t_total, array_size, num_threads);

  return 0;
}

void *sort_thread(void *arg)
{
  SortParams *params = (SortParams *)arg;
  bubble_sort(params->arrays, params->start, params->end, params->array_size);
  return NULL;
}

void bubble_sort(int **arrays, int start, int end, int size)
{
  for (int i = start; i < end; i++)
  {
    int *array = arrays[i];
    for (int j = 0; j < size - 1; j++)
    {
      for (int k = 0; k < size - j - 1; k++)
      {
        if (array[k] > array[k + 1])
        {
          int temp = array[k];
          array[k] = array[k + 1];
          array[k + 1] = temp;
        }
      }
    }
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

// #define IMPRIME

struct bubble_args {
  int *array;
  unsigned int size;
};

void imprime(int **array, int num_arrays, int size){
  int i,j;
  for(i=0; i < num_arrays; i++){
     for(j=0; j < size; j++){
        printf("%d ", array[i][j]);
     }
     printf("\n");
  }
}

void *bubble(void *arg){
  struct bubble_args *args = (struct bubble_args *)arg;
  int *array = args->array;
  unsigned int size = args->size;
  int i,j;
  int temp;
  for(i = 0; i < size-1; i++){       
    for(j = 0; j < size-i-1; j++){          
      if(array[j] > array[j+1]){               
        temp = array[j];
        array[j] = array[j+1];
        array[j+1] = temp;
      }
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char **argv){
  int **elementos,n,tam,i,j;    
  struct timeval t1, t2;
  double t_total;

  if(argc!=3){
   printf("Digite %s Num_arrays Num_elementos\n", argv[0]);
   exit(0);
  }

  n = atoi(argv[1]);
  tam = atoi(argv[2]);

  pthread_t threads[n];

  /*Aloca memória para os vetores*/
  elementos=(int**)malloc(n*sizeof(int*));
  for(i = 0; i < n; i++)
    elementos[i]=(int*)malloc(tam*sizeof(int));

  /*Popula os arrays com elementos aleatorios entre 0 e 1000*/
  for(i = 0; i < n; i++) {
    for (j = 0; j < tam; j++)
      elementos[i][j] = rand() % 1000;    
  }

  #ifdef IMPRIME
  printf("Antes da ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  struct bubble_args *args = malloc(n * sizeof(struct bubble_args));
  gettimeofday(&t1, NULL);
  for(i = 0; i < n; i++){
    args[i].array = elementos[i];
    args[i].size = tam;
    pthread_create(&threads[i], NULL, bubble, &args[i]);    
  }

  for(i = 0; i < n; i++){
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&t2, NULL);


  #ifdef IMPRIME
  printf("Depois da ordenacao!\n");
  imprime(elementos, n, tam);
  printf("\n");
  #endif

  /*Libera memoria alocada*/
  for(i=0;i<n;i++){
		free(elementos[i]);
	}
	free(elementos);

  t_total = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec)/1000000.0;
  printf("%i; %f; %i\n", n, t_total, tam);

  return 0;
}


#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 2
int global = 0;

void *mais5(void *threadid){
	global += 5;

	printf("\tThread %d: Acrescentando 5 à variàvel. Valor final: %i\n", threadid, global); fflush(stdout);
	
	pthread_exit(NULL); 
}

void *mais1(void *threadid){
	global += 1;
	printf("\tThread %d: Acrescentando 1 à variàvel. Valor final: %i\n", threadid, global); fflush(stdout);
	
	pthread_exit(NULL); 
}
	
int main(){

	pthread_t threads[NUM_THREADS];
	int t = 0;

	printf("Criando thread para adicionar 1....\n");
	pthread_create(&threads[0], NULL, mais1, (void *)t);

	t = 1;
	printf("Criando thread para adicionar 5....\n");
	pthread_create(&threads[1], NULL, mais5, (void *)t);
	
	for(t=0; t < NUM_THREADS; t++)
		pthread_join(threads[t],NULL); /* wait for all the threads to terminate*/

}
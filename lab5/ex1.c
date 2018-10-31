#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS 2

void *contDecrescente(void *threadid){
	int i = 30;

	while (i != 0){
		printf("Thread %d: %i\n", threadid, i); fflush(stdout);
		i--;
		sleep(2);
	}
	pthread_exit(NULL);
}

void *contCrescente(void *threadid){
	int i = 20;

	for(int j = 1; j <= i; j++){
		printf("\tThread %d: %i\n", threadid, j); fflush(stdout);
		sleep(1);
	}
	pthread_exit(NULL); 
}
	
int main(){

	pthread_t threads[NUM_THREADS];
	int t = 0;

	printf("Criando thread para contar decrescente....\n");
	pthread_create(&threads[0], NULL, contDecrescente, (void *)t);

	t = 1;
	printf("Criando thread para contar cresente....\n");
	pthread_create(&threads[1], NULL, contCrescente, (void *)t);
	
	for(t=0; t < NUM_THREADS; t++)
		pthread_join(threads[t],NULL); /* wait for all the threads to terminate*/

}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define NUM_THREADS 2
#define NUM_NUM 64
#define MAX_FILA 8

int *lista;
int id_cauda;
int id_cabeca;
int n_elementos;
int produtor_terminado = 0;


void init(){
	lista = (int*) malloc (sizeof(int)*MAX_FILA);
	id_cabeca = 0;
	id_cauda = 0;
	n_elementos = 0;
	srand( time(NULL) );

}

void *retira(void *threadid){
	for (int i = 0; i < NUM_NUM; i++){

		if (id_cabeca == id_cauda && produtor_terminado) break;

		printf("\tConsumidor: ");		

		do{
			printf("%i ", lista[id_cabeca]); fflush(stdout);
			lista[id_cabeca] = -1;
			id_cabeca++;

			if (id_cabeca > (MAX_FILA-1)) id_cabeca = 0;
			n_elementos--;



		} while (id_cabeca != id_cauda);


		printf("\n");

		sleep(2);
	}
	printf("Consumidor terminado\n");
	pthread_exit(NULL); 
}

void *insere(void *threadid){
	for (int i = 0; i < NUM_NUM; i++){
		int x = rand()%100;
		
		lista[id_cauda] = x;

		id_cauda++;
		if (id_cauda > (MAX_FILA - 1)) id_cauda = 0;
		n_elementos++;

		if(n_elementos > MAX_FILA){
			printf("FILA CHEIA");
			exit(0);	
		} 
		
		printf("Produtor: %d\n", x); fflush(stdout);

		sleep(1);

	}

	printf("Produtor terminado\n");
	produtor_terminado = 1;
	pthread_exit(NULL); 
}

int main(){
	init();
	pthread_t threads[NUM_THREADS];
	int t = 0;

	printf("Criando produtor....\n");
	pthread_create(&threads[0], NULL, insere, (void *)t);

	t = 1;
	printf("Criando consumidor....\n");
	pthread_create(&threads[1], NULL, retira, (void *)t);
	
	for(t=0; t < NUM_THREADS; t++)
		pthread_join(threads[t],NULL); /* wait for all the threads to terminate*/

	printf("Terminado\n");

	free(lista);

}
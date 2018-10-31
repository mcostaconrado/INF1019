#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 9
#define X 150
#define Y 5
#define D 500

int distancias[N];
int classificacao[N];
int descansos[N];
int pulos[N];

int corrida_acontecendo;
int sapos_terminados;

void init(){
	for (int i = 0; i < N; i++){
		distancias[i] = 0;
		classificacao[i] = -1;
	}
	corrida_acontecendo = 1;
	sapos_terminados = 0;
}

void sapo (void *threadid){
	int pulo = 0;
	int descanso = 0;
	
	while(descanso == 0 || pulo == 0){
		pulo = rand()%X;
		descanso = rand()%(Y/2) + Y/2;
	}

	int distancia_percorrida = 0;
	descansos[(int)threadid] = descanso;
	pulos[(int)threadid] = pulo;

	sleep(5);

	while(distancia_percorrida < D){
		distancia_percorrida += pulo;
		distancias[(int)threadid] = distancia_percorrida;
		if (distancia_percorrida >= D) break;
		sleep(descanso);
	}

	classificacao[sapos_terminados] = threadid;
	sapos_terminados++;
}

void controle (void *threadid){
	
	sleep(4);
	system("clear");
	while(corrida_acontecendo){

		for (int i = 0; i < N; i++){
			printf("O sapo numero %d pula com distancia %d e descansa %d segundos.\n", i+1, pulos[i], descansos[i]);
		}
		
		printf("\nLINHA DE CHEGADA: %d\n\n", D);

		for (int i = 0 ; i < N; i++){

			float porcentagem = (distancias[i]*1.0)/D*100;

			printf("Sapo %02d: ", i+1);

			for (int j = 0; j < 40*distancias[i]/D && j < 40; j++){
				putchar('.');fflush(stdout);
			}

			if (porcentagem > 100) porcentagem = 100;

			if (distancias[i] > D) distancias[i] = D;
			printf(" %d (%.0f %)", distancias[i], porcentagem);
			printf("\n");fflush(stdout);

		}

		printf("\n");fflush(stdout);
		sleep(1);
		system("clear");
		
	}
}

int main(){

	init();

	srand(time(NULL));
	pthread_t threads[N+1];
	system("clear");

	for(int t=0;t < N;t++)
		pthread_create(&threads[t], NULL, sapo, (void *)t);

	pthread_create(&threads[N], NULL, controle, (void *)N);

	sleep(1);
	printf("PREPARAR\n");
	sleep(1);
	printf("APONTAR\n");
	sleep(1);
	printf("JA!!\n");

	for(int t=0; t < N; t++)
		pthread_join(threads[t],NULL); /* wait for all the threads to terminate*/

	sleep(1);

	corrida_acontecendo = 0;

	pthread_join(threads[N],NULL);

	printf("CORRIDA DE SAPO TERMINADA!!!\n\nClassificacao:\n");

	for (int i = 0; i < N; i++)
		printf("%i.o lugar: Sapo %i\n", (i+1), (classificacao[i]+1));
}
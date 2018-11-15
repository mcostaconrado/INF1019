#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
/*
	ENUNCIADO

1) Usando processos, escreva um programa C que implemente o problema do produtor/consumidor.
Existem 2 consumidores. O produtor e os consumidores devem compartilhar os recursos do
processo (filas e demais dados). O produtor deve produzir dados (números inteiros) a cada 1
segundo colocando-os em uma fila (buffer). Os consumidores devem retirar dados da fila a cada 2
segundos. O tamanho máximo da fila deve ser de 8 elementos (MAXFILA) e tanto o produtor como
os dois consumidores devem produzir/consumir 64 elementos.*/

#define MAXFILA 8
#define N_ELEMENTOS 64

int produz(){	//produz numero aleatório de 0 a 50
	srand = time(NULL);
	int n = rand()%51;
	printf("Produzindo item: %i\n", n);
	return ;
}

void consome(int item, int n){
	for (int i = 0; i < n)
		printf("\t");
	
	printf("Consumidor %n recebeu um item: %i\n", n, item);

}

void produtor (){
	int item;
	mensagem msg;

	while(n < 64){
		item = produz();
		receive(consumidor, &msg);

	}
}



int main(){

	int pid_filho;

	if ((pid_filho = fork()) == 0){ //filho



	}
	else{ // pai

	}


}
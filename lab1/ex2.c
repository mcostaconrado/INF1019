/*
ENUNCIADO

Usando a mesma estrutura de processos pai e filho, declare e inicialize uma variável
com valor 1 antes do fork( ), isto é, visível ao pai e ao filho. No pai, faça um loop de
1 a 50 que some 1 à variável e imprima seu valor e em seguida espere o filho
terminar. No filho, faça um loop de 1 a 100 que some 2 à variável e imprima o seu
valor. Em seguida o filho termina. Há concorrência? O que se pode dizer com relação
à variável inicializada e aos valores impressos? Justifique os resultados obtidos.

*/


#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

	int var = 1;
	int status;
	int pid;
	
	if ((pid = fork()) != 0){ //pai
		
		while (var <= 50){
		
			printf ("PAI: %d\n", var);
			var++;
		}
		
		printf("\n");
		
		printf("esperando o filho terminar");
		
		printf("\n\n");
		
		waitpid(pid, &status, 0);
		
		printf("filho terminou. terminando...");
		

	}
	
	else{
	
		while (var <= 100){
		
			printf ("\tFILHO: %d\n", var);
			var += 2;
		}
	
		
	
	}



}

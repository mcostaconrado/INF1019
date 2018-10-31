/*
ENUNCIADO

	Faça o mesmo problema do item 2, agora com o pai gerando o filho e o filho gerando
	um filho (portanto o neto do processo pai). Este neto adiciona 3 à variável em um
	loop de 1 a 150. Há concorrência? O que se pode dizer com relação à variável
	inicializada? Justifique os resultados obtidos.

*/


#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

	int var = 1;
	int status;
	int pid1, pid2;
	
	if ((pid1 = fork()) != 0){ //pai
		
		while (var <= 50){
		
			printf ("PAI: %d\n", var); fflush(stdout);
			var++;
		}
		
		printf("\n");
		
		printf("pai diz: esperando o filho terminar\n"); fflush(stdout);
		
		printf("\n\n");
		
		waitpid(pid1, &status, 0);
		
		printf("pai diz: filho terminou. terminando...\n"); fflush(stdout);
	}
	
	else{
		
		if ((pid2 = fork()) != 0){ // filho
		
			while (var <= 100){
		
				printf ("\tFILHO: %d\n", var); fflush(stdout);
				var += 2;
			}
			
			printf("\tfilho diz: esperando o neto terminar\n"); fflush(stdout);
			waitpid(pid2, &status, 0);
			printf("\tfilho diz: neto terminou. terminando...\n"); fflush(stdout);
			
			
		}
		else{ // neto
		
			while (var <= 150){
		
				printf ("\t\tNETO: %d\n", var); fflush(stdout);
				var += 3;
			}	
			printf("\t\tneto diz: neto terminando\n"); fflush(stdout);
		}

	}
}

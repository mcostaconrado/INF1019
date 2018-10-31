/*
ENUNCIADO

DESAFIO: Faça o mesmo problema do item 3, agora com o pai gerando 2 filhos
(portanto no mesmo nível). Faça um resumo sobre as diferenças verificadas na
execução dos programas dos itens 3 e 4.

*/


#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

	int var = 1;
	int status;
	int pid1, pid2;
	
	if ((pid1 = fork()) != 0){ /*pai*/

		if((pid2=fork()) != 0){

			//PAI

			while (var != 150){
				printf( "PAI: %d\n", var); fflush(stdout);
				var++;
			}

			waitpid (-1, &status, 0);
		}

		else { //FILHO 1

			while (var != 150){
				printf( "\tFILHO 1: %d\n", var); fflush(stdout);
				var++;
			}

		}
	}

	else{ /*filho 2*/

		while (var != 150){
			printf( "\t\tFILHO 2: %d\n", var); fflush(stdout);
			var++;
		}
	}
	return 0;
}

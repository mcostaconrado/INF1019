#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>	
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <string.h>

int main(int argc, char* argv[]){

	int segmento;
	int key = (atoi(argv[0]));
	char *p;

	/*ALOCAÇÃO DA MEMÓRIA: 

		int shmget (key_t key, size_t size, int shmflg);

		key: chave de identificação da memória (se souber qual é) ou IPC_PRIVATE para gerar um identificador novo
 		size: quantidade MÍNIMA de memória a ser alocada
 		shmflg: modo de criação
	*/
	
	p = (char*) shmat(key, 0, 0);

	printf("\n[PROGRAMA 2] A MENSAGEM DO DIA É \n\t>> ");

	printf("%s\n\n", p);

	shmdt(p); //DESANEXANDO;

	shmctl(key, IPC_RMID, 0); //DESALOCANDO MEMÓRIA

	return 0;
}
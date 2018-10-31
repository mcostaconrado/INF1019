#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>	
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <string.h>

int main(){

	//char* mensagem;
	char mensagem[256];
	int segmento;
	char arg[13];
	char *call[2] = {NULL, NULL};
	char *p;

	/*ALOCAÇÃO DA MEMÓRIA: 

		int shmget (key_t key, size_t size, int shmflg);

		key: chave de identificação da memória (se souber qual é) ou IPC_PRIVATE para gerar um identificador novo
 		size: quantidade MÍNIMA de memória a ser alocada
 		shmflg: modo de criação
	*/

	segmento = shmget(IPC_PRIVATE, sizeof(char)*100, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO); // creat entry if key does not exist | owner can write only | others can read only
	
	p = (char*) shmat(segmento, 0, 0);

	snprintf(arg, 13, "%d", segmento);

	printf("[PROGRAMA 1] ESCREVA A MENSAGEM DO DIA: \n\t>> ");
	scanf("%[^\n]s", p);

	call[0] = arg;



	shmdt(p); //DESANEXANDO;
	execve( "./ex1B", call, 0);

	//shmctl(segmento, IPC_RMID, 0); //DESALOCANDO MEMÓRIA -> SO FAÇO ISSO NO OUTRO PROGRAMA

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>	
#include <sys/wait.h>
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS

int main(void){
	


	int segmento;

	segmento = shmget(8752, 100*sizeof(char), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	char *mensagem = (char*)shmat(segmento, 0, 0);

	scanf("%[^\n]s", mensagem);



}
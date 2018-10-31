#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>	
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <time.h>
#include <limits.h>
#define MAX_LINE 256
#define KEY 8732

typedef struct proc{	//ESTRUTURA DE UM PROCESSO
	char nome[100];
	int pid;
	int prioridade;
	int ut_restante;
	int ut_seguido;		// há quantas uts seguidas o processo está executando
	int tempo_bloqueado;
} Processo;

int *pid_escalonador;
Processo *shm_processo;


void interpreta_linha(char* line){
	char *exec, *prog, *prio_c;
	int prio;

	exec = strtok (line," "); //exec
	prog = strtok (NULL, " "); //nome do programa
	prio_c = strtok (NULL, " ");
	prio = prio_c[11] - '0'; //prioridade=i

	//printf("%s %i", prog, prio);

	strcpy(shm_processo->nome, prog);
	shm_processo->prioridade = prio;


	kill(*pid_escalonador, SIGUSR1);
}

void trata_arquivo (const char* filename){
	size_t len = 0;
	char read;

	char* line = NULL;

	FILE* exec_file = fopen(filename, "r");
	while ((read = getline(&line, &len, exec_file)) != -1) {
		clock_t inicio = clock();
		interpreta_linha(line);

		for (; ;){
			if ( (clock() - inicio)/CLOCKS_PER_SEC >= 1 ) break; 
		}
	}

	fclose(exec_file);
}


int main(int argc, char* argv[]){
	const char *filename = "exec.txt";

	int key1 = atoi(argv[0]);
	int key2 = atoi(argv[1]);
	pid_escalonador = (int*) shmat(key1, 0, 0);
	shm_processo = (Processo*) shmat(key2, 0, 0);

	//printf("PID do escalonador = %d\n\n", *pid_escalonador);

	
	trata_arquivo(filename);
	//printf("-- FIM DO INTERPRETADOR -- \n");


	shmdt(pid_escalonador); //DESANEXANDO;
	shmdt(shm_processo);
	shmctl(key1, IPC_RMID, 0); //DESALOCANDO MEMÓRIA
	shmctl(key2, IPC_RMID, 0);
	return 0;
}
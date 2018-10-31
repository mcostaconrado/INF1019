/*
ENUNCIADO: 

	Faça um programa para criar dois processos (fork()), o pai escreve seu pid e espera
o filho terminar (waitpid()), o filho escreve o seu pid e termina (exit()). Pesquise uma
função do sistema que indique o pid de um processo. Comprove através do pid o
comportamento das chamadas ao sistema fork( ), waitpid( ) e exit( ). Descreva suas
conclusões.*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	int pid;
	int pid_pai;
	int pid_filho;
	int status;
	
	printf("\n");
	
	pid = fork();
	
	if ( pid != 0){ //CÓDIGO DO PAI
	
		pid_pai = getpid();
		/*printf("pai falando. pid_filho = %d\n", pid_filho);*/
		printf("PAI FALANDO: \n\n");
		
		printf("Meu PID: %d\n", pid_pai);
		printf("PID do meu filho: %d\n", pid);
		printf("esperando filho terminar\n\n\n");
		
		
		waitpid(pid, &status, 0);
		
		
		
		printf("PAI FALANDO: \n");
		printf("filho terminou. terminei\n\n");
		
	}
	else{ //CÓDIGO DO FILHO
		/*printf("filho falando. pid_filho = %d\n", pid_filho);*/
		pid_filho = getpid();
		printf("\tFILHO FALANDO: \n\n");
		printf("\tMeu PID: %d\n", pid_filho);
		printf("\tTerminei \n\n\n");
			
	}
	return 0;
}

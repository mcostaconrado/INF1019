#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>



int main(){

	int fd[2];
	int pid;
	int status;

	if (pipe(fd) < 0){
		printf("Erro ao criar pipe.\n");
		return -1;
	}



	if ( (pid = fork()) == 0){//filho
		close(fd[0]);
		char texto[] = "Oi, eu sou o filho";

		write(fd[1], texto, sizeof(texto) + 1);

		sleep(2);

		strcpy(texto,  "outra mensagem...");
		write(fd[1], texto, sizeof(texto) + 1);


		close(fd[1]);

	}
	else{ //pai
		char texto[256];
		close(fd[1]);
		//waitpid(-1, &status, 0);


		read(fd[0], texto, sizeof(texto) + 1);

		printf("Pai lendo a mensagem: '%s'\n", texto);

		//waitpid(-1, &status, 0);
		//sleep(2);

		read(fd[0], texto, sizeof(texto) + 1);
		printf("Pai lendo a mensagem: '%s'\n", texto);

		waitpid(-1, &status, 0);

		close(fd[0]);

	}
}
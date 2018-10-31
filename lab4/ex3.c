#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>


int main(){

	int fd[2];
	int pid;
	char* arg[1] = {"/home/ccosta/Desktop/INF1019/lab4/ex3.txt"};
	int status;

	if (pipe(fd) < 0){
		printf("Erro ao criar pipe.\n");
		return -1;
	}



	if ( (pid = fork()) == 0){//filho
		execve("gedit", arg , NULL);
		
	}
	else{ //pai
		


		waitpid(-1, &status, 0);
	}
		



}
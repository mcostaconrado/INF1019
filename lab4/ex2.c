#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


int main(){

	int fd1;
	int fd2;

	int retorno1, retorno2;

	if ( (fd1 = open("entrada.txt", O_RDWR | O_CREAT | O_TRUNC, 0222)) == -1){
		printf("Erro ao abrir pipe\n");
		return -1;
	}
	
	if ( (fd2 = open("saida.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) == -1){
		printf("Erro ao abrir pipe\n");
		return -1;
	}

	
	close(0);
	if ( (retorno1 = dup(fd1)) == -1) {
		perror("Error open()");
		return -1;
	}

	if ( (retorno2 = dup2(fd2,1)) == -1) {
		perror("Error open()");
		return -1;
	}

	int a; int b;
	//scanf("%s", s);
	scanf("%d", &a);
	scanf("%d", &b);

	//printf("%s", s);
	printf("Soma: %i\n", a + b);
	printf("Produto: %i\n", a * b);

}
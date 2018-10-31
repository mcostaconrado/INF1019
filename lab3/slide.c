#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


#define EVER ;;
/*
void intHandler (int signal);
void quitHandler (int signal);

int main(void){


	void (*p) (int);
	p = signal (SIGFPE, intHandler);
	printf("Endereço do manipulador anterior: %p \n",p);
	p = signal (SIGQUIT, quitHandler);
	printf("Endereço do manipulador anterior: %p \n",p);
	puts ("Ctrl-c desabilitado. Use ctrl-\\ para terminar \n");

	int c;

	printf("insira um divisor para 3: ");
	scanf("%i", &c);

	int d = 3/c;

	printf("%d \n", getpid());

	for (EVER){
		puts(".\n");
	}
} 

void intHandler (int signal){
	printf("Você dividiu por zero, mano (%d)\n",signal);
	//exit(0);
}

void quitHandler(int signal){
	printf("Você pressionou ctrl-\\ (%d). Terminando o processo.\n", signal);
	exit(0);
}
*/

int main(){

	printf("%d \n", getpid());
	for(EVER){
		puts(".\n");
	}
	puts("continuei\n"); fflush(stdout);

}
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>	
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <string.h>
#include <time.h>

#define POS 128


int acha_posicao (int *v, int pos_inicial){
	int maior_valor = 0;
	int maior_pos = pos_inicial;
	int i;

	for (i = pos_inicial; i < pos_inicial + 16; i++){
		if (v[i] > maior_valor){
			maior_valor = v[i];
			maior_pos = i;
		}
	}

	return maior_pos;
}

int main(int argc, char* argv[]){

	int i;
	srand(time(NULL));
	int pid[8];
	int meupid;

	int segmento1, segmento2, *p, *q;

	segmento1 = shmget(IPC_PRIVATE, sizeof(int)*128, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	segmento2 = shmget(IPC_PRIVATE, sizeof(int)*8, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	p = (int*)shmat(segmento1, 0, 0);
	q = (int*)shmat(segmento2, 0, 0);

	for (i = 0; i < POS; i++){
		p[i] = rand()%500;
	}

	for (i = 0; i < 8; i++){
		pid[i] = fork();

		if (pid[i] == 0) break;
	}

	if (pid[0] == 0){
		int maior_pos = acha_posicao (p, 0*128/8);
		q[0] = maior_pos;

	}
	else if (pid[1] == 0){
		int maior_pos = acha_posicao (p, 1*128/8);
		q[1] = maior_pos;
	}
	else if (pid[2] == 0){
		int maior_pos = acha_posicao (p, 2*128/8);
		q[2] = maior_pos;
	}
	else if (pid[3] == 0){
		int maior_pos = acha_posicao (p, 3*128/8);
		q[3] = maior_pos;
	}
	else if (pid[4] == 0){
		int maior_pos = acha_posicao (p, 4*128/8);
		q[4] = maior_pos;
	}
	else if (pid[5] == 0){
		int maior_pos = acha_posicao (p, 5*128/8);
		q[5] = maior_pos;
	}
	else if (pid[6] == 0){
		int maior_pos = acha_posicao (p, 6*128/8);
		q[6] = maior_pos;	
	}
	else if (pid[7] == 0){
		int maior_pos = acha_posicao (p, 7*128/8);
		q[7] = maior_pos;
	}
	else{	//pai;
		int maior = 0;
		int maiorpos = 0;
		printf("Eu sou o pai. O vetor aleatório está aí: \n\n");

		for (i = 0; i < 128; i++){
			printf("%i ", p[i]); fflush(stdout);
			if (!(i+1)%16) printf("\n");
		}

		printf("\n\n");

		printf("Cada filho meu achou esses valores como sendo o maior de suas respectivas partes: \n\n");
		for (i = 0; i < 8; i++){
			printf("%i " , p[q[i]]);
			if (p[q[i]] > maior){
				maior = p[q[i]];
				maiorpos = q[i];
			}
		}

		printf("\n\nO maior numero, portanto, é v[%i] = %i.\n\n", maiorpos, maior);
	}
	shmdt(p);
	shmdt(q);
	shmctl(segmento1, IPC_RMID, 0);
	shmctl(segmento2, IPC_RMID, 0);
}

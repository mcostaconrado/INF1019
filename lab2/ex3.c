#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>	
#include <sys/ipc.h> //FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> //FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <string.h>


int main(){
	int segmento1, segmento2, *matriz, *transposta, status;
	int i, j;
	int pid[4];

	segmento1 = shmget(IPC_PRIVATE, sizeof(int)*16, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	matriz = (int*)shmat(segmento1, 0, 0);
	segmento2 = shmget(IPC_PRIVATE, sizeof(int)*16, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	transposta = (int*)shmat(segmento2, 0, 0);


	for (i = 0; i < 16; i++){
		scanf("%i", &matriz[i]);
	}

	/*for (i = 0; i < 16; i++){
		//for (i = 0; i < 4; i++){
		printf("%i ", matriz[i]);
		//}
		//printf("\n");
	}*/

	for (i = 0; i < 4; i++){
		pid[i] = fork();

		if (pid[i] == 0) break;
	}

	if (pid[0] == 0){
		transposta[0] = matriz[0];
		transposta[4] = matriz[1];
		transposta[8] = matriz[2];
		transposta[12] = matriz[3];

	}
	else if (pid[1] == 0){
		transposta[1] = matriz[4];
		transposta[5] = matriz[5];
		transposta[9] = matriz[6];
		transposta[13] = matriz[7];
	}
	else if (pid[2] == 0){
		transposta[2] = matriz[8];
		transposta[6] = matriz[9];
		transposta[10] = matriz[10];
		transposta[14] = matriz[11];
	}
	else if (pid[3] == 0){
		transposta[3] = matriz[12];
		transposta[7] = matriz[13];
		transposta[11] = matriz[14];
		transposta[15] = matriz[15];
	}
	else{


		waitpid(-1, &status, 0);


		printf("\n\nMATRIZ: \n\n");
		for (i = 0; i < 16; i++){
			printf("%i ", matriz[i]);
			if (!((i+1)%4)) printf("\n");
		}


		printf("\n\n");

		printf("TRANSPOSTA: \n\n");

		for (i = 0; i < 16; i++){
			printf("%i ", transposta[i]);
			if (i > 0 && !((i+1)%4)) printf("\n");
		}
	}


	shmdt(matriz);
	shmctl(segmento1, IPC_RMID, 0);
	shmdt(transposta);
	shmctl(segmento2, IPC_RMID, 0);
}
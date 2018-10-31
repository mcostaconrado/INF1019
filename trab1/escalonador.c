#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>	
#include <sys/ipc.h> 	//FLAGS DE MODO DE CRIAÇÃO
#include <sys/stat.h> 	//FLAGS DE MODO DE CRIAÇÃO -> PERMISSÕES DE USUÁRIOS
#include <time.h>
#include <limits.h>
#include <errno.h>

#define N_PRIORIDADES 7
#define LIMITE_UT 5
#define FATIA_UT_RR 3
#define IOBOUND_INT 1	// FREQUENCIA DE BLOQUEIO
#define IOBOUND_BLOQ 3	// TEMPO DE BLOQUEIO POR I/O

typedef struct proc{	// ESTRUTURA DE UM PROCESSO
	char nome[100];		
	int pid;			
	int prioridade;		
	int ut_restante;	// quantos UT ainda restam
	int ut_seguido;		// há quantas uts seguidas o processo está executando
	int tempo_bloqueado;
} Processo;

typedef struct no_lista{
	Processo *p;
	struct no_lista *prox;
}No;

typedef struct fil{		//ESTRUTURA DE UMA FILA
	No *processos;		
	No *inicio;			// ponteiro para o primeiro da fila
	No *final;			// ponteiro para o último da fila
	int qt_elementos;	// quantos processos daquela prioridade estão sendo escalonados
} Fila;

void mostra_fila (Fila *f);
void devolve_fila_io();	

/************* VARIÁVEIS GLOBAIS *************/

int pid_interpretador;			// variável para armazenar o PID do interpretador
int *pid_escalonador;			// variável para armazenar o PID do escalonador
int qt_processos_total = 0;		// variável para indicar quantos processos estão sendo escalonados no momento.

Processo *shm_processo;			// variável de memória compartilhada para pegar processos do interpretador
Fila **vet_filas;				// este vetor contem N_PRIORIDADES filas

Processo *executando = NULL;	// variável para indicar qual processo está executando no momento
Processo *escolhido = NULL;		// variável para indicar qual processo foi escolhido para executar


/************* \VARIÁVEIS GLOBAIS *************/

Fila** cria_filas(){
	Fila **f = (Fila**)malloc(sizeof(Fila*)*(N_PRIORIDADES+1));	//  crio N_PRIORIDADES+1 para que o índice de cada vetor de prio seja a 																própria prioridade (e não prio-1)
	for (int i = 0; i <= N_PRIORIDADES; i++){
		f[i] = (Fila*)malloc(sizeof(Fila));
		f[i]->processos = NULL;									// lista vazia.
		f[i]->inicio = NULL;									// lista vazia.
		f[i]->final = NULL;										// lista vazia.
	}
	return f;
}

void escolhe_processo(){	// a cada ut, esta função é chamada, a fim de ver qual processo terá o direito de ser executado.

	for (int i = 1; i < N_PRIORIDADES +1; i++){		// percorro o vetor de listas da menor para a maior prioridade...

		if (vet_filas[i]->inicio){					// verificando se há processo na lista i

			escolhido = vet_filas[i]->inicio->p;	// coloca o processo na variável de escolhido

			return;									// break para parar de procurar assim que encontrar.
		}
	}
	escolhido = NULL;
}

void insere_fila(Fila* f, Processo *p){

	No *aux_no = (No*)malloc(sizeof(No));
	Processo *aux_p = (Processo*) malloc(sizeof(Processo));

	strcpy(aux_p->nome,p->nome);
	aux_p->prioridade = p->prioridade;
	aux_p->pid = p->pid;
	aux_p->ut_restante = LIMITE_UT;
	aux_p->ut_seguido = 0;

	aux_no->p = aux_p;
	aux_no->prox = NULL;

	if(f->qt_elementos == 0){	// se for o primeiro a ser adicionado...
		f->inicio = aux_no;		// início aponta para este mesmo processo
		f->final = aux_no;		// final também
	}
	else{

		f->final->prox = aux_no;						// o último processo ganha um outro para ser seu próximo.
		f->final = aux_no;								// o ponteiro para o final passa a apontar para o processo recém adicionado.
		if (aux_p->prioridade == 6 || aux_p->prioridade == 7)	// se for round robin ...
			aux_no->prox = f->inicio;							// o último da fila tem, como próximo, o primeiro da fila
		else 											// se não for round robin ...
			aux_no->prox = NULL;

	}

	(f->qt_elementos)++;	// adicionei um elemento, então atualizo a variável de controle do nº de processos na fila...
	qt_processos_total++;	/// .. e no total
	printf("[PROCESSO %s INSERIDO NA FILA CORRESPONDENTE]\n\n", p->nome);

}

void remove_fila(Processo *p, int prio){
	int i; No *aux;
	/*for (i = 1; i < N_PRIORIDADES + 1; i++){
		if (p == vet_filas[i]->inicio->p) break;
	}*/
	if (prio == 6 || prio == 7){		// se quiser remover da fila de round robin

/********** UPDATE 2:46. REMOÇÃO DO QUE ESTÁ COMENTADO E INSERÇÃO DO NOVO BLOCO. ***********/

		/*
		No *aux = vet_filas[prio]->inicio;	// preciso de um ponteiro apontando para o primeiro processo
		No *aux_2 = vet_filas[prio]->final;	// e outro apontando pro final

		vet_filas[prio]->inicio = vet_filas[prio]->inicio->prox;	// como vou retirar o primeiro, o ponteiro de início, agora, aponta 															   para o início->prox
		vet_filas[prio]->final->prox = vet_filas[prio]->inicio;
		//aux_2->prox = vet_filas[prio]->inicio;						// o último agora aponta para o novo início.
		free(aux);													// desaloco o nó do processo retirado
		*/

		No *aux = vet_filas[prio]->inicio;

		if(vet_filas[prio]->qt_elementos > 1 ){
			vet_filas[prio]->inicio = vet_filas[prio]->inicio->prox;
			//vet_filas[6]->final->prox = vet_filas[6]->inicio; 
		}
		else{
			vet_filas[prio]->inicio = NULL;
			vet_filas[prio]->final = NULL;
		}
	}
	else{
		/*
		aux = vet_filas[prio]->inicio;
		vet_filas[prio]->inicio = vet_filas[prio]->inicio->prox;

		if (vet_filas[prio]->final->p == p) vet_filas[prio]->final = NULL;
		free(aux);
		*/

		No *aux = vet_filas[prio]->inicio;

		if (vet_filas[prio]->qt_elementos == 1){
			vet_filas[prio]->inicio = NULL;
			vet_filas[prio]->final = NULL;
		}
		else{
			vet_filas[prio]->inicio = vet_filas[0]->inicio->prox;
		}

/********** \ UPDATE 2:46. REMOÇÃO DO QUE ESTÁ COMENTADO E INSERÇÃO DO NOVO BLOCO.***********/
	}
	
	vet_filas[prio]->qt_elementos = vet_filas[prio]->qt_elementos - 1;
	qt_processos_total--;
	

}

void mostra_fila (Fila *f){
	No *aux = f->inicio;

	for (int i = 0; i < f->qt_elementos; i++){
		printf("%s[%i][%i][%i] --> ", aux->p->nome, aux->p->pid, aux->p->ut_restante, aux->p->tempo_bloqueado); fflush(stdout);
		aux=aux->prox;
	}

	printf("null\n\n");
}

void mostra_vet_filas (Fila **f){
	for (int i = 0; i < N_PRIORIDADES + 1; i++){
		if (i == 0)
			printf("WAITING: ");
		else 
			printf("PRIO %i: ", i);

		mostra_fila(f[i]);
	}
}

void destroi_filas(){
	for (int i = 0; i <= N_PRIORIDADES + 1; i++){
		free (vet_filas[i]);
	}
	free (vet_filas);
}

void handler_novoProcesso(int sinal){
	//printf("Escalonador diz: RECEBI UM PROCESSO!\n");fflush(stdout);
	int pid;
	//printf("Nome do processo: %s\n", shm_processo->nome);
	//printf("Prioridade do processo: %i\n", shm_processo->prioridade);

	if ((pid = fork()) < 0 ){
		printf("erro ao criar processo.\n");
		exit(-2);
	}
	else if (pid == 0){ 
		char aux[100] = "./";
		char *call[2] = {NULL, NULL};
		strcat(aux, shm_processo->nome);
		execve(aux, call, 0);
	}

	shm_processo->pid = pid;
	int prio = shm_processo->prioridade;
	insere_fila(vet_filas[prio], shm_processo);
	//printf("PID do processo: %i\n", shm_processo->pid);		

	kill(shm_processo->pid, SIGSTOP);

	
}

void tira_fila_io(){

	No *aux = vet_filas[6]->inicio;
	//aux->prox = NULL;

	if(vet_filas[6]->qt_elementos > 1 ){
		vet_filas[6]->inicio = vet_filas[6]->inicio->prox;
		//vet_filas[6]->final->prox = vet_filas[6]->inicio; 
	}
	else{
		vet_filas[6]->inicio = NULL;
		vet_filas[6]->final = NULL;
	}

	(vet_filas[6]->qt_elementos)--;

	if (vet_filas[0]->qt_elementos == 0){
		aux->prox = NULL;
		vet_filas[0]->inicio = aux;
		vet_filas[0]->final = aux;

	}
	else{
		aux->prox = NULL;
		vet_filas[0]->final->prox = aux;
		vet_filas[0]->final = vet_filas[0]->final->prox;
		//vet_filas[0]->final->prox = NULL;
	}

	(vet_filas[0]->qt_elementos)++;
	
}

void devolve_fila_io(){

	No *aux = vet_filas[0]->inicio;
	//aux->prox = NULL;


	if (vet_filas[0]->qt_elementos == 1){
		vet_filas[0]->inicio = NULL;
		vet_filas[0]->final = NULL;
	}
	else{
		vet_filas[0]->inicio = vet_filas[0]->inicio->prox;
	}


	(vet_filas[0]->qt_elementos)--;

	if (vet_filas[6]->qt_elementos == 0){
		aux->prox = vet_filas[6]->inicio;
		vet_filas[6]->inicio = aux;
		vet_filas[6]->final = aux;
	}
	else{
		aux->prox = vet_filas[6]->inicio;
		vet_filas[6]->final->prox = aux;
		vet_filas[6]->final = vet_filas[6]->final->prox;
		//vet_filas[0]->final->prox = NULL;
	}

	(vet_filas[6]->qt_elementos)++;

}

void atualiza_bloqueios_io(){
	No *aux = vet_filas[0]->inicio;

	for (int i = 0; i < vet_filas[0]->qt_elementos; i++){
		(aux->p->tempo_bloqueado)--;
		aux = aux->prox;
	}
}

void move_fimfila_RR(int prio){
	if(vet_filas[prio]->qt_elementos > 1){
		No* aux = vet_filas[prio]->inicio;
		vet_filas[prio]->inicio = vet_filas[prio]->inicio->prox;

		aux->prox = vet_filas[prio]->inicio;
		vet_filas[prio]->final->prox = aux;	
		vet_filas[prio]->final = aux;
		
	}
}



/************* FUNÇÕES DE CONTROLE DE PROCESSOS *************/

void interrompe_processo(Processo *p) {
////printf("Interrompendo %s. PID = %d\n",  p->nome, p->pid);
	kill(p->pid, SIGSTOP);	//ENVIA SIGINT PARA PROCESSO
}

void executa_processo(Processo *p) {
	//printf("Executando %s. PID = %d\n",  p->nome, p->pid);
	//printf("%s\n", p->nome);
	kill(p->pid, SIGCONT);				//ENVIA SIGCONT PARA PROCESSO
}

/************* \FUNÇÕES DE CONTROLE DE PROCESSOS *************/

void inicializa_interpretador(){

	char *call[3] = {NULL, NULL, NULL};
	char arg1[13], arg2[13];

	int segmento_pid_escalonador = shmget(IPC_PRIVATE, sizeof(char)*100, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO); 
	pid_escalonador =  shmat(segmento_pid_escalonador, 0, 0);
	*pid_escalonador = getpid();
	snprintf(arg1, 13, "%d", segmento_pid_escalonador);

	call[0] = arg1;

	int segmento_shm_processo = shmget(IPC_PRIVATE, sizeof(char)*100, IPC_CREAT | IPC_EXCL | S_IRWXU | S_IRWXG | S_IRWXO); 
	shm_processo =  shmat(segmento_shm_processo, 0, 0);
	snprintf(arg2, 13, "%d", segmento_shm_processo);

	call[1] = arg2;

	if((pid_interpretador = fork()) == 0)
		execve("./interpretador", call, 0);

	shmdt(pid_escalonador);
}

int main(){

	
	int ciclos = 0;
	clock_t inicio = clock();

	signal(SIGUSR1, handler_novoProcesso);

	vet_filas = cria_filas();
	inicializa_interpretador();

	/* UPDATE 00:48: AGORA, EU GARANTO QUE SEMPRE O PROCESSO É COLOCADO NA FILA NO SEGUNDO ANTERIOR À EXECUÇÃO DO ESCALONADOR. PROGRAMA MAIS ESTÁVEL. */

	for (; ;){
		if ( (clock() - inicio) >= CLOCKS_PER_SEC/5 ) break; 
	}

	/* FIM DO UPDATE */

	while (qt_processos_total || !ciclos){	// eqt existir processos ou o escalonador n tiver rodado nenhuma vez (lista vazia no inicio)

		int count = 0;	// variável de controle 
		//system("clear");
		
		inicio = clock();

		printf("\t\t>>>>>>> SEGUNDO %i A %i <<<<<<< \n\n", ciclos, ciclos+1);

		while(!escolhido){		// para que evite que o escalonador pegue antes do interpretador mandar o primeiro.
			if (!count || !ciclos){
				
				if (qt_processos_total){
					printf("AS FILAS ESTÃO DA SEGUINTE FORMA:\n\n");
					mostra_vet_filas(vet_filas);
					escolhe_processo();
				}	
			}
			else
				break;
			count++;
		}

		if(!escolhido){
			printf("NÃO HÁ NENHUM PROCESSO PRONTO PARA SER EXECUTADO. CPU OCIOSA\n\n");
			for (; ;){
				if ( (clock() - inicio)/CLOCKS_PER_SEC >= 1 ) break; 
			}
		}
		else{

			if (!executando && escolhido) {		// se não tinha nenhum processo executando e um foi escolhido
				executando = escolhido;
				executa_processo(escolhido);
				inicio = clock();
			}
			else if (escolhido != executando){	// se o processo escolhido é diferente do que estava executando antes
				interrompe_processo(executando);
				if (executando->prioridade == 6 || executando->prioridade == 7){
					executando->ut_seguido = 0;
					move_fimfila_RR(executando->prioridade);
				}
				executa_processo(escolhido);
				executando = escolhido;
				inicio = clock();
			}

			printf("\nEXECUTANDO: %s", executando->nome);
			printf("\nPID: %i", executando->pid);
			printf("\nPRIORIDADE: %i\n\n", executando->prioridade);


			for (; ;){
				if ( (clock() - inicio)/CLOCKS_PER_SEC >= 1 ) break; 
			}


			(executando->ut_restante)--;
			(executando->ut_seguido)++;

			if (executando->prioridade == 7){ //CPU BOUND
				if (executando->ut_seguido == FATIA_UT_RR){
					executando->ut_seguido = 0;
					move_fimfila_RR(executando->prioridade);
				}
			}

			if (executando->prioridade == 6){	//IO BOUND
				if (executando->ut_seguido == IOBOUND_INT){
					executando->tempo_bloqueado = IOBOUND_BLOQ + 1; //mais um porque, no final, eu atualizo os boqueios.
					executando->ut_seguido = 0;
					tira_fila_io();
				}
			}
			else {
				if ((executando->ut_restante) <= 0) {
					interrompe_processo(executando);
					remove_fila(executando, executando->prioridade);
					executando = NULL;
				}
			}

		}

		/* AQUI, SE EXISTIR UM PROCESSO I/O BOUND DESBLOQUEADO, ELE VOLTA PARA SUA FILA. */

		atualiza_bloqueios_io();

		if (vet_filas[0]->qt_elementos > 0){
			No *aux_io = vet_filas[0]->inicio;
			if(aux_io->p->tempo_bloqueado == 0){
				if (aux_io->p->ut_restante == 0){
					remove_fila(aux_io->p, 0);
				}
				else{
					devolve_fila_io();
				}
			}
		}

		/* \AQUI, SE EXISTIR UM PROCESSO I/O BOUND DESBLOQUEADO, ELE VOLTA PARA SUA FILA. */

		escolhido = NULL;
		ciclos++;

		//mostra_vet_filas(vet_filas);
	}

	printf("\n\n\nTODOS OS PROCESSOS TERMINARAM\n\nFIM DO ESCALONADOR DE PROCESSOS\n");
	destroi_filas();

	return 0;
}
/***
* Projeto 2 - MC504 1s2014
* Aluno: Leonardo de Oliveira Silva RA: 108330
* Descricao: implementação de uma animação para representar 
* o problema do Papai Noel apresentado pelo livro
* The Litle Book on Semaphores
*
*
*Todos os desenhos retirados de: http://www.geocities.com/spunk1111/xmas.htm
****/

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

/* Cores */
#define red   "\033[0;31m"
#define green "\033[0;32m"      
#define none   "\033[0m"        
#define brown  "\033[0;33m"

/* Constantes */
#define N_RENAS 9			/* numero de renas que estao de ferias */
#define N_DUENDES 3			/* numero de duendes necessários para acordar o papai noel */
#define QTDE_DUENDES 100     		/* numero de duendes que trabalham pro papai noel */
#define DUENDE 0
#define RENA 1
#define NOEL 3

pthread_mutex_t 
	mutex, mutex_rena, mutex_duende,  mutex_escrita;

sem_t 
	sem_papai_noel, sem_rena, sem_duende;

volatile int 
	esta_acordado, 		/* estado do papai noel */
	duendes_ajudados, 	/* quantidade de duendes que conseguiram ser ajudados */
	duendes,		/* variável contadora de duendes */
	renas;			/* variável contadora de renas */

/* Leva o cursor a uma posição x, y da tela */
void gotoxy(int x,int y) 
{
	printf("%c[%d;%df",0x1B,y,x);
}

/* Apresentação inicial */
void iniciar() 
{
	system("clear");
	printf("___________________________________________\n");
	printf("|    ___ ___ _  _ _____ ___      .----,   |\n");
	printf("|   / __\\ . \\ \\| |_   _| . \\    /____, \\  |\n");
	printf("|   \\__ \\   |  ` | | | |   |   {_____}  O |\n");
	printf("|   \\___/_|_|_|\\_| |_| |_|_|  (/ . . \\)   |\n");
	printf("|      ___ _    ___ _ _ ___   { -=^=- }   |\n");
	printf("|     /  _/ |  | . \\ | | __/  {       }   |\n");
	printf("|    |  (_  |__|   | | |__ \\  {       }   |\n");
	printf("|   _ \\___\\____|_|_|___/___/   {     }    |\n");
	printf("|  (_)_______                    -,-      |\n");
	printf("|  |/| Polo  |       MC504                |\n");
	printf("|  |/| Norte |       Prof. Islene         |\n");
	printf("|  |/|--------       1s2014               |\n");
	printf("|  |/|               Papai Noel           |\n");
	printf("|_________________________________________|\n");
	printf("MELHOR VISUALIZADO EM TELA CHEIA\n");
	printf("Pressione Enter para continuar...\n");
	getchar();
}

/* Função para desenhar n duendes, um ao lado do outro, e também exibir informaçoes sobre os duendes */
void desenharDuendes(int n) 
{
	int i, x, y, y0;
	y0 = 20;
	y = 0;
	gotoxy(0,y0-2);
	printf("NUMERO DE DUENDES QUE PAPAI NOEL JA AJUDOU: %d\n",duendes_ajudados);
	gotoxy(0,y0-1);
	printf("NUMERO DE DUENDES PRECISANDO DE AJUDA: %d\n",duendes);
	printf("%s",green);
	for (i = 0; i < n; i++) {
		x = 10 * (i % 15);
		if (i % 15 == 0 && i != 0) 
			y += 3;
		gotoxy(x,y+y0);
		printf("    ___,@ \n");
		gotoxy(x,y+1+y0);
		printf("   /  <   \n");
		gotoxy(x,y+2+y0);
		printf("  /    \\\n");
	}
	printf("%s",none);
}


/* Funçao para desenhar n renas, uma ao lado da outra, e também exibir informações sobre as renas */
void desenharRenas(int n) 
{
	int i, x, y, y0;
	y0 = 30;
	y = 0;
	gotoxy(0,y0-1);
	printf("RENAS QUE VOLTARAM DAS FERIAS: %d\n",renas);
	printf("%s",brown);
	for (i = 0; i < n; i++) {
		x = 13 * (i % 12);
		if (i % 12 == 0 && i != 0) 
			y += 5;
		gotoxy(x,y+0+y0);
		printf("\\       /\n");
		gotoxy(x,y+1+y0);
		printf("_\\/   \\/_\n");
		gotoxy(x,y+2+y0);
		printf(" _\\'\"'/_\n");
		gotoxy(x,y+3+y0);
		printf(" (_, ,_)\n");
	}
	printf("%s",none);
}  

/* Função para desenhar o Papai Noel de acordo com o seu estado */
void desenharPapaiNoel()
{
	printf("            /             \\\n");
	printf("           /_______________\\\n");
	printf("          [_________________]\n");
	if (esta_acordado)
		printf("           / (_o_)   (_o_) \\ \n");
	else 
		printf("           / (,,,)   (,,,) \\ zzzzzzzzzzzzzzzzzzzzzzz\n");
	printf("          (______,   ,______)\n");
	printf("          /     .'---'.     / \n");
	printf("         /`~\"\"'`  ._.  `'\"\"~`\\ \n");
	printf("        (                     )\n");
	printf("       (                       )\n");
	printf("      (                         )\n");
	printf("      (                         )\n");
	printf("       (                       )\n");
	printf("        (                     )\n");
	printf("         `(                 )'\n");
	printf("           `\"~'---,.,---'~\"`\n");
}

/* Finaliza a animação quando todas as renas voltam de férias */
void finalizar() 
{
	pthread_mutex_lock(&mutex_escrita);
	system("clear");
	printf("                                                    __.  .--,\n");
	printf(" *-/___,  ,-/___,-/___,-/___,-/___,           _.-.=,{\\/ _/  /`)\n");
	printf("  `\\ _ ),-/___,-/___,-/___,-/___, )     _..-'`-(`._(_.;`   /\n");
	printf("   /< \\\\=`\\ _ )`\\ _ )`\\ _ )`\\ _ )<`--''`     (__\\_________/___,\n");
	printf("          /< <\\ </ /< /< /< </ /<           (_____Y_____Y___,\n");  
	printf("\n\n\n\n");
	printf("\tJA TENHO O NUMERO DE RENAS SUFICIENTE PARA PARTIR!\n");
	printf("\tPapai noel ja foi embora mas...\n");
	printf("\tajudou %d duendes\n",duendes_ajudados);
	printf("\tabandonou %d duendes\n", duendes);
	printf("\n\n\n\n");
	exit(0);
	pthread_mutex_unlock(&mutex_escrita);
}

/* Monta a tela principal */
void exibir() 
{
	pthread_mutex_lock(&mutex_escrita);
	system("clear");
	desenharPapaiNoel();
	desenharDuendes(duendes);
	desenharRenas(renas);
	sleep(1);
	pthread_mutex_unlock(&mutex_escrita);
}

/* Principal função para a thread do papai noel */
void *noel_thread(void* arg)
{  
	while(1) {

		exibir();
		sem_wait(&sem_papai_noel);
		esta_acordado = 1;
		exibir();

		/* Quando o Papai Noel é acordado, ele verifica se 
		foi acordado pelos duendes ou pelas renas, lembrando
		que a prioridade é das renas sempre. */

		/* Caso ele tenha sido acordado pelas renas,
		ele parte para a viagem e a animação é finalizada. */
		if (renas == N_RENAS) {
			pthread_mutex_unlock(&mutex_escrita);
			finalizar();
			return NULL;
		}

		/* Caso ele tenha sido acordado pelos duendes,
		ele vai ajudá-los, mas sem esquecer das renas. */
		else if (duendes == N_DUENDES) {
			while (duendes > 0) {
				pthread_mutex_lock(&mutex_duende);
				if (renas == N_RENAS) {
					pthread_mutex_unlock(&mutex_escrita); 
					finalizar(); 
					return NULL;
				}
				--duendes;
				++duendes_ajudados;
				exibir();
				pthread_mutex_unlock(&mutex_duende);
			}
			/* Libera a chegada de novos duendes depois de ajudar todos */
			sem_post(&sem_duende);
		}
		/* Volta a dormir */
		esta_acordado = 0;   
  	}	 
  return NULL;
}

/* Função principal para a thread de uma rena */
void *rena_thread(void *arg) {
	
	/* As renas voltam aos poucos das férias.
	Quando todas chegarem, o Papai Noel é acordado */
	
	pthread_mutex_lock(&mutex_rena);
	sleep(1);
	renas++;
	exibir();
	if (renas == N_RENAS) {
		sem_post(&sem_papai_noel); 
		return NULL;
	}
	pthread_mutex_unlock(&mutex_rena);
	return NULL;
}

/* Função principal para a thread de um duende */
void *duende_thread(void *arg) {

	/* Um duende espera até que todos os que chegaram primeiro que ele
	tenham obtido ajuda, antes dele ir pedir ajuda. */

	sem_wait(&sem_duende);
	pthread_mutex_lock(&mutex_duende);
	++duendes;
	sleep(1);
	exibir();
	
	/* Quando o último duende chega, o papai noel é acordado. */
	if (duendes == N_DUENDES) {
		sem_post(&sem_papai_noel);
		pthread_mutex_unlock(&mutex_duende);
		return NULL;
	} 
	pthread_mutex_unlock(&mutex_duende);
	sem_post(&sem_duende);
	return NULL;
}

int main(void) {

	pthread_t noelT;
	pthread_t renasT[N_RENAS];
	pthread_t duendesT[QTDE_DUENDES];

	int id_rena[N_RENAS];
	int id_duende[QTDE_DUENDES];
	int i;
	
	/* Inicialização das variáveis contadoras e auxiliares */
	duendes_ajudados = 0;
	duendes	= 0;
	renas = 0;
	esta_acordado = 0;	/* Papai Noel começa dormindo */

	/* Inicialização dos semáfaros */
	sem_init(&sem_papai_noel, 0, 0);
	sem_init(&sem_rena, 0, 1);
	sem_init(&sem_duende, 0, 1);

	/* Inicialização dos mutexes */
  pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex_escrita, NULL);
	pthread_mutex_init(&mutex_rena, NULL);
	pthread_mutex_init(&mutex_duende, NULL);

	/* Apresentação inicial */
	iniciar();

	/* Criação das threads */
  	pthread_create(&noelT, NULL, noel_thread, NULL); 
	for(i = 0; i < QTDE_DUENDES; i++){
 		id_duende[i] = i;
  		pthread_create(&duendesT[i], NULL, duende_thread, (void*) &id_duende[i]);
	}
  	for(i = 0; i < N_RENAS; i++){
	    	id_rena[i] = i;
	    	pthread_create(&renasT[i], NULL, rena_thread, (void*) &id_rena[i]);
  	}

	/* Espera pelas threads */
  	pthread_join(noelT, NULL);
	for(i = 0; i < QTDE_DUENDES; i++) {
  		pthread_join(duendesT[i], NULL);
	}
  	for(i = 0; i < N_RENAS; i++) {
    		pthread_join(renasT[i], NULL);
	}

	/* Finalização */

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_escrita);
	pthread_mutex_destroy(&mutex_rena);
	pthread_mutex_destroy(&mutex_duende);
	sem_destroy(&sem_papai_noel);
	sem_destroy(&sem_rena);
	sem_destroy(&sem_duende);
	printf("%s",none); /* limpa a cor */
	return 0;
}

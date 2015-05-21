#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "pthread.h"

using namespace std;

pthread_mutex_t cont_atras = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion_atras = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cont_adelante = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion_adelante = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cont_par = PTHREAD_MUTEX_INITIALIZER;

void *contarasc(void*);
void *contardes(void*);

#define lim_max 10
#define lim_min 1

main()
{
	pthread_t h1, h2;

	pthread_create(&h1, NULL, &contarasc, NULL);
	pthread_create(&h2, NULL, &contardes, NULL);
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);

	exit(0);
}

void *contarasc(void *sinuso)
{
	int cont1 = 1;
	for (;;)
	{
		pthread_mutex_lock(&cont_adelante);
		do
		{
			cout <<"Hilo 1: " <<cont1 <<endl;
			cont1 ++;
		}while(cont1%2 != 0);

		pthread_cond_wait(&condicion_atras, &cont_adelante);
		pthread_cond_signal (&cont_adelante);
		pthread_mutex_unlock(&cont_adelante);

		if (cont1 >= lim_max) return (NULL);
	}
}


void *contardes(void *sinuso)
{
	int cont = 1;
	for (;;)
	{
		pthread_mutex_lock(&cont_atras);
		do
		{
			cout <<"Hilo 1: " <<cont <<endl;
			cont --;
		}while(cont%2 != 0);
		pthread_cond_signal (&cont_atras);
		pthread_cond_wait(&condicion_adelante, &cont_atras);
		pthread_mutex_unlock(&cont_atras);

		if (cont <= lim_min) return (NULL);
	}
}
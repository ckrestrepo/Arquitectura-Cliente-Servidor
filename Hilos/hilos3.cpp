#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "pthread.h"

using namespace std;

/***************************/
 /*  LLAMADO A FUNCIONES   */
/***************************/
// g++ hilos.cpp -lpthread -o "nombre"

void *mensaje(void *ptro);
void *contador(void *);

int main()
{
	pthread_t h1, h2;
	char *msj1 = "Hilo 1";
	char *msj2 = "Hilo 2";
	int iret1, iret2;
	int n = 4, val;
	iret1 = pthread_create (&h1 , NULL , mensaje , (void*) msj1);
	iret2 = pthread_create (&h2 , NULL , &contador , &n);

	pthread_join(h1, NULL);
	printf("Hilo 1 retorna: %d\n", iret1);
	pthread_join(h2, (void**)&val);
	printf("Hilo 2 retorna: %d\n", val);
	exit(0);
}


/*******************************/
 /*  DEFINICION DE FUNCIONES   */
/*******************************/


void *mensaje(void *ptro)
{
	char *msj;
	msj = (char *) ptro;
	printf("%s \n", msj);
	//cout << msj;
}

void *contador(void *ptro)
{
	int n = *((int*)ptro);
	int suma = 0;
	for (int i = 0; i < n; ++i)
	{
		suma = suma+i;
	}
	return (void*) suma;
}
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

void * funcion1(void *);
void * funcion2(void *);


// Para compilar
// g++ hilos.cpp -lpthread -o "nombre"
int main()
{
	pthread_t h1, h2;

	pthread_create (&h1 , NULL , &funcion1 , NULL );
	pthread_create (&h2 , NULL , &funcion2 , NULL );
	printf(" Cargando hilos \n" );
	pthread_join(h1, NULL);
	pthread_join(h2, NULL);
	cout <<"\nfin de hilos\n";
}





/*******************************/
 /*  DEFINICION DE FUNCIONES   */
/*******************************/
void *funcion1(void * arg)
{
	cout <<"\nCorriendo el hilo 1...\n";
	return NULL;
}

void *funcion2(void * arg)
{
	for (int i = 0; i < 5; ++i)
	{
		cout << i<< endl;
	}
	return NULL;
}
Holaa

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

#define MAX 128

void *invertir(void *ptro);
void *vocales(void *ptro);

int main()
{
	pthread_t h1, h2;
	int iret1, iret2;
	char pal [MAX];
	cout <<"Digite una palabra: ";
	cin >> pal;
	iret1 = pthread_create (&h1 , NULL , invertir , (void*) pal);
	iret2 = pthread_create (&h2 , NULL , vocales , (void*) pal);

	pthread_join(h1, NULL);
	//printf("Hilo 1 retorna: %d\n", iret1);
	pthread_join(h2, NULL);
	//printf("Hilo 2 retorna: %d\n", iret2);
	exit(0);
}


/*******************************/
 /*  DEFINICION DE FUNCIONES   */
/*******************************/


void *invertir(void *ptro)
{
	char *msj;
	msj = (char *) ptro;
	cout <<"\nPalabra Invertida\n";
	int longitud = strlen(msj);
	for (int i = longitud-1; i >= 0; i--)
	{
		cout << msj[i];
	}
	cout <<endl;
}


void *vocales(void *ptro)
{
	char *msj, *msj2;
	msj = (char *) ptro;
	int longitud = strlen(msj);
	int cont = 0;
	for (int i = 0; i < longitud; ++i)
	{
		if ((msj[i] == 'a') || (msj[i] == 'e') ||(msj[i] == 'i') ||(msj[i] == 'o') ||(msj[i] == 'u'))
		{
			cont++;
		}
	}
	cout <<endl <<msj <<" tiene " << cont <<" vocales" <<endl;
}
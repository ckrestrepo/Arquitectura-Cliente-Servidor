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

void *impares(void *);
void *asterisco(void *ptro);

int main()
{
	pthread_t h1, h2;
	int iret1, iret2;
	int num1, val1;
	cout <<"Digite un numero: ";
	cin >> num1;
	iret1 = pthread_create (&h1 , NULL , &impares , &num1);
	do
	{
		pthread_join(h1, (void**)&val1);

	} while (val1 == 0);

	iret2 = pthread_create (&h2 , NULL , &asterisco , &val1);

	pthread_join(h2, (void**)&val1);

	//printf("Hilo 2 retorna: %d\n", val1);
	exit(0);
}

void *impares(void *ptro)
{
	int n1 = *((int*)ptro);
	int suma = 0, impar;
	cout <<"\n...Asteriscos...\n";
	for (int i = 1; i <= n1; ++i)
	{
		if (i%2 != 0)
		{
			impar = i;
		}	
	}
	return (void*) impar;
}


void *asterisco(void *ptro)
{
	int n = *((int*)ptro);
	for (int i = 1; i <= n; ++i)
	{
		cout << "*";
	}
	cout <<endl;
}

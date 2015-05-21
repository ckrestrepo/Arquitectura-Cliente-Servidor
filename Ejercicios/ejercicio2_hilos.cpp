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

void *pares(void *);
void *sumatoria(void *);

int main()
{
	pthread_t h1, h2;
	int iret1, iret2;
	int num1, num2, val1, val2;
	cout <<"Digite el primer numero (h1): ";
	cin >> num1;
	cout <<"Digite el segundo numero (h2): ";
	cin >> num2;
	iret1 = pthread_create (&h1 , NULL , &pares , &num1);
	iret2 = pthread_create (&h2 , NULL , &sumatoria , &num2);

	pthread_join(h1, (void**)&val1);
	//printf("Hilo 1 retorna: %d\n", val1);
	pthread_join(h2, (void**)&val2);
	printf("Sumatoria hilo 2: %d\n", val2);
	exit(0);
}

void *pares(void *ptro)
{
	int n1 = *((int*)ptro);
	int suma = 0;
	cout <<"\nNumeros pares...\n";
	for (int i = 1; i <= n1; ++i)
	{
		if (i%2 == 0)
		{
			cout << i <<endl;
		}	
	}
}

void *sumatoria(void *ptro)
{
	int n2 = *((int*)ptro);
	int suma = 0;
	for (int i = 1; i <= n2; ++i)
	{
		suma = i+suma;
	}
	return (void*) suma;
}
#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "pthread.h"
#include "list"

using namespace std;

// g++ hilos.cpp -lpthread -o "nombre"

void *mcd(void *ptro);

int main()
{
	pthread_t h1, h2;
	int iret1, iret2;
	int num1, num2, val1;
	cout <<"Digite el primer numero (h1): ";
	cin >> num1;
	cout <<"Digite el segundo numero (h2): ";
	cin >> num2;
	
	int mensaje[2]= {num1, num2};

	iret1 = pthread_create (&h1 , NULL , &mcd ,  &mensaje);
	pthread_join(h1, (void**)&val1);
	exit(0);
}


void *mcd(void *ptro)
{
	int *msj;
	msj = (int *) ptro;
	int a = msj[0];
	int b = msj[1];
	int mcd;
	while(a != b)
	{
		if (a > b)
		{
			a = a - b;
		}
		else
		{
			b = b - a;
		}
	}
	mcd = a;
	cout <<"MCD: " << mcd <<endl;
}

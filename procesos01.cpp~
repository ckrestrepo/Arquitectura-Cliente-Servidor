#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "sys/types.h"
#include "wait.h"

using namespace std;

int main()
{
	pid_t pid;
	int estadoHijo;
	int valor = 10, numero;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit (-1);
	}
	if (pid == 0)
	{
		//Este es el hijo
		cout <<"Digite un numero: ";
		cin >> numero;
		if (numero > 100)
		{
			exit(1);
		}
		else
		{
			exit(2);
		}
	}
	if (pid > 0)
	{
		// Este es el padre
		sleep(1);
		wait(&estadoHijo);
		if (WEXITSTATUS(estadoHijo) == 1)
		{
			cout <<"El hijo digito un numero mayor a 100...\n";
			cout <<"ID del padre: " <<getppid() <<endl;
		}
		else
		{
			cout <<"El hijo digito un numero Menor a 100...\n";
			cout <<"ID Propia: " <<getpid() <<endl;
		}
	}	
}

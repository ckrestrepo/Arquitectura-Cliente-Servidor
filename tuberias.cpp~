/* ..... SERVIDOR CON PROCESOS ....*/

#include "stdio.h"
#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "sys/types.h"
#include "wait.h"
#include "sstream"

using namespace std;

#define tam 126

string inttostring(int);

int main(int argc, char *argv[])
{
	pid_t pid;
	int tub[2], bytesleidos;
	char memoria[tam];
	pipe(tub);
	pid = fork();
	if (pid == 0)
	{
		close(tub[0]); //Cierro el lado de lectura
		int a, b, res;
		//Hijo
		cout <<"Digite el primer numero: ";
		cin >> a;
		cout <<"Digite el segundo numero: ";
		cin >> b;
		res = a + b;
		string mensaje = "La suma da: " + inttostring(res);
		strcpy(memoria, mensaje.c_str());
		write(tub[1], memoria, strlen(memoria));
		close(tub[1]);
	}
	else
	{
		//Padre
		sleep(1);
		close (tub[1]);  //Cierro el lado de escritura
		while((bytesleidos = read(tub[0], memoria, tam)) > 0)
		{
			memoria[bytesleidos] = '\0';
			string msj = (string)memoria;
			cout <<"...Mensaje del hijo...\n" <<msj <<endl;
		}
	}
	waitpid(pid, NULL, 0);
	exit(0);
}

//Convierte de entero a cadena
string inttostring(int x)
{
    stringstream ss;
    ss<<x;
    string cadena = ss.str();
    return cadena;
}

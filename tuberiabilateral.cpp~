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
	int tuba[2], tubb[2], bytesleidos; // Dos tuberias para hijo y para padre
	char memoria[tam];
	//Declaro las tuberias
	pipe(tuba);
	pipe(tubb);	
	
	pid = fork();
	
	if (pid == 0)
	{
		close(tuba[1]); //Cierro el lado de lectura
		close(tubb[0]); //Cierro el lado de Escritura
		string menh = "";
		//cout <<"\nProceso hijo corriendo...\n";
		
		//Bloqueo hasta que llegue el mensaje
		while((bytesleidos = read(tuba[0], memoria, tam)) > 0)
		{
			memoria[bytesleidos] = '\0';
			string menp = (string)memoria;
			cout <<"Mensaje del padre: " <<menp <<endl;
		}
		close(tuba[0]);
		memoria[0] = '\0';
		
		strcpy(memoria, "Recibido\n");
		write(tubb[1], memoria, strlen(memoria));
		close(tubb[1]);
	}
	else
	{
		// Padre
		close(tuba[0]); // Cierro el lado de escritura
		close(tubb[1]); // Cierro el lado de lectura
		string menp = "";
		cout <<"Digite el mensaje: ";
		cin >> menp;
		strcpy(memoria, menp.c_str());
		write(tuba[1], memoria, menp.size());
		close(tuba[1]);
		while((bytesleidos = read(tubb[0], memoria, tam)) > 0)
		{
			memoria[bytesleidos] = '\0';
			string menh = (string)memoria;
			cout <<"Mensaje del hijo: " <<menh <<endl;
		}
		close(tubb[0]);
	}
	waitpid(pid, NULL, 0);
	exit(0);
}
		
		
		/*
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
*/
//Convierte de entero a cadena
string inttostring(int x)
{
    stringstream ss;
    ss<<x;
    string cadena = ss.str();
    return cadena;
}

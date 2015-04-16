/* ..... SERVIDOR CON TUBERIAS ....*/

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
int stringtoint(string);


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
		int a, b;		
		cout <<"\nParte uno hijo\n";
		//Bloqueo hasta que llegue el mensaje
		while((bytesleidos = read(tuba[0], memoria, tam)) > 0)
		{
			cout <<"\nParte dos hijo dentro del while\n";
			memoria[bytesleidos] = '\0';
			string numa = (string)memoria;
			cout <<"Mensaje del padre: " <<numa <<endl;
			a = stringtoint(numa);
		}
		cout <<"\nParte tres hijo fuera del while\n";
		memoria[0] = '\0';
		strcpy(memoria, "Numero recibido\n");
		write(tubb[1], memoria, strlen(memoria));
		while((bytesleidos = read(tuba[0], memoria, tam)) > 0)
		{
			memoria[bytesleidos] = '\0';
			string numb = (string)memoria;
			cout <<"Mensaje del padre: " <<numb <<endl;
			b = stringtoint(numb);
		}
		close(tuba[0]);
		int res = a + b;
		string resul = "Suma: " + inttostring(res);
		memoria[0] = '\0';
		strcpy(memoria, resul.c_str());
		write(tubb[1], memoria, strlen(memoria));
		close(tubb[1]);
	}
	else
	{
		// Padre
		close(tuba[0]); // Cierro el lado de escritura
		close(tubb[1]); // Cierro el lado de lectura
		string numa, numb;
		cout <<"Digite el primer numero: ";
		cin >> numa;
		strcpy(memoria, numa.c_str());
		write(tuba[1], memoria, numa.size());
		cout <<"\nParte uno padre\n";
		//close(tuba[1]);
		while((bytesleidos = read(tubb[0], memoria, tam)) > 0)
		{
			cout <<"\nParte dos padre dentro del while\n";
			memoria[bytesleidos] = '\0';
			string menh = (string)memoria;
			cout <<"Mensaje del hijo: " <<menh <<endl;
		}
		cout <<"\nParte tres padre fuera del while\n";
		//close(tuba[1]);
		cout <<"Digite el segundo numero: ";
		cin >> numb;
		strcpy(memoria, numb.c_str());
		write(tuba[1], memoria, numb.size());
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

int stringtoint(string x)
{
    int valor = atoi(x.c_str());
    return valor;
}
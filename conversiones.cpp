#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
using namespace std;

//libreria de conversiones
int stringtoint(string);
string stringtoint(int);
string doubletostring(double);
double stringtodouble(string);

int suma(int, int);


int main()
{
	cout <<"\n...Seccion de Pruebas...\n";
	string palabra;
	double suma = 0, n = 0, promedio;
	do
	{
		cout <<"Digite un numero: ";
		cin >> palabra;
		if (palabra != "fin")
		{
			suma = suma + stringtodouble(palabra);
			n++;
		}
	}while(palabra != "fin");
	promedio = suma / n;
	cout <<"Promedio en double: " << promedio <<endl;
	cout <<"Promedio en String: " + doubletostring(promedio) <<endl;
}
// Convierte de cadena a entero
int stringtoint(string x)
{
	int valor = atoi(x.c_str());
	return valor;
}

//Convierte de entero a cadena
string inttostring(int x)
{
	stringstream stream;
	stream << x ;
	string cadena = stream.str();
	return cadena;
}

string doubletostring(double x)
{
    stringstream ss;
    ss<<x;
    string cadena = ss.str();
    return cadena;
}

double stringtodouble(string x)
{
	double valor = atof(x.c_str());
	return valor;
}

int suma(int a, int b)
{
	int resul = a + b;
	return resul;
}
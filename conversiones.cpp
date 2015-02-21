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
int suma(int, int);


int main()
{
	string numstring, numstring2;
	int res;
	cout <<"Digite un numero: ";
	cin >> numstring;
	cout <<"Digite otro numero: ";
	cin >> numstring2;
	res = suma(stringtoint(numstring), stringtoint(numstring2));
	//Suma de numeros
	cout <<"La suma es: " << res <<endl;

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

int suma(int a, int b)
{
	int resul = a + b;
	return resul;
}
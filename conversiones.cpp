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

//Funciones de conversion

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


int main()
{
	int numint, numint2;
	string numstring, numstring2;
	cout <<"\nPrimera prueba Int\n";
	cout <<"Digite un numero: ";
	cin >> numint;
	numstring2 = inttostring(numint);
	cout <<"Numero convertido a String: " << numstring2;
	cout <<"\nSegunda prueba String\n";
	cout <<"Digite una cadena (numero): ";
	cin >> numstring;
	numint2 = stringtoint(numstring);
	cout <<"Cadena convertida a Int: " << numint2;
	cout <<"\n!!!Nos vemos en la proxima!!!\n";
}
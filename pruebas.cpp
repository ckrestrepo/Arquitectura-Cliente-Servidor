#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "list"

using namespace std;

//libreria de conversiones
int stringtoint(string);
string stringtoint(int);
string doubletostring(double);
double stringtodouble(string);
//Metodos
int suma(int, int);


struct persona
{
	string nombre;
	string apellido;
};

void mostrar(list<int>);
list<int> leer();
list<int> eliminar(list<int>);
void mostrarp(list<persona>);
list<persona> leerp();
list<persona> eliminar(list<persona>);

int main()
{
	list<int> lista;
	lista = leer();
	mostrar(lista);
	lista = eliminar(lista);
	mostrar(lista);
}

//-----------------------------------------------------------------------//
// ....................... AREA DE FUNCIONES ............................//
//-----------------------------------------------------------------------//

void mostrar(list<int> l)
{
	cout <<"Cantidad de datos: " <<l.size() <<endl;
	list <int>:: iterator i;
	for (i=l.begin(); i!= l.end(); i++)
	{
		cout <<"Dato: "<< *i << endl;
	}	
}

list<int> leer()
{
	int valor, opcion;
	list <int> ls;
	do
	{
		cout <<"Digite un numero: ";
		cin >> valor;
		ls.push_back(valor);
		cout <<"Desea ingresar mas datos... <Si=1 - No=0: ";
		cin >> opcion;
	}while(opcion != 0);
	return ls;
}

list<int> eliminar(list<int> ls)
{
	ls.remove(10);
	return ls;
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
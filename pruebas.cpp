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
	string usuario;
	string pw;
	//string dirIP;
};

void mostrar(list<int>);
list<int> leer();
list<int> eliminar(list<int>);
void mostrarp(list<persona>);
list<persona> leerp();
list<persona> registros;
bool autenticarR(list<persona>, string, string);
int main()
{
	bool existe;
	string us, p;
	registros = leerp();
	mostrarp(registros);
	cout <<"Autenticar registro...\n";
	cout <<"Digite el usuario: ";
	cin >> us;
	cout <<"Digite la contraseña: ";
	cin >> p;
	existe = autenticarR(registros, us, p);
	if (existe == true)
	{
		cout <<"Usuario con acceso al sistema\n";
	}
	else
	{
		cout <<"Usuario NO TIENE ACCESO\n";
	}

}

//-----------------------------------------------------------------------//
// ....................... AREA DE FUNCIONES ............................//
//-----------------------------------------------------------------------//

list<persona> leerp()
{
	persona p;
	int opcion;
	list <persona> per;
	do
	{
		cout <<"Digite el nombre: ";
		cin >> p.nombre;
		cout <<"Digite el usuario: ";
		cin >> p.usuario;
		cout <<"Digite la contraseña: ";
		cin >> p.pw;
		per.push_back(p);
		cout <<"Desea ingresar mas personas... <Si=1 - No=0: ";
		cin >> opcion;
	}while(opcion != 0);
	return per;
}
void mostrarp(list<persona> per)
{
	cout <<"Cantidad de personas: " <<per.size() <<endl;
	list <persona>:: iterator i = per.begin();
	while (i!= per.end())
	{
		persona p;
		p =*i;
		cout <<"Nombre: "<< p.nombre<< endl;
		cout <<"usuario: "<< p.usuario <<endl;
		cout <<"Password: "<< p.pw<< endl;
		i++;
	}	
}

bool autenticarR(list<persona> per, string user, string pass)
{
	//cout <<"Cantidad de registros: " <<per.size() <<endl;
	list <persona>:: iterator i = per.begin();
	int y = 1;
	while (i!= per.end())
	{
		persona p;
		p =*i;
		if ((p.usuario == user) && (p.pw == pass))
		{
			
			cout <<"If\n";
			cout <<"Ciclo: " << y <<endl;
			cout <<p.usuario <<endl;
			cout <<p.pw <<endl;
			return true;
		}
		i++;
	}	
}






// Espacio para lista de enteros
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
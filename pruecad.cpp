
#include "iostream"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sstream"

#define MAXLONGITUD 128

using namespace std;

string doubletostring(double);

int main()
{
	char cad[MAXLONGITUD];
	double suma = 0, n = 0, promedio, x;
	string completo = "", palabra = "", mensaje;
	while (palabra != "0")
	{
		cout <<"Digite un numero o cero para sacar promedio: ";
		cin >> palabra;
		completo = completo + palabra+"-";
	}
	cout << "completo: " <<completo <<endl;
	strcpy(cad, completo.c_str());
	char *numero;
	numero = strtok (cad, "-");
	while(numero != NULL)
	{
		cout << "Numero: " <<numero <<endl;	
		if (numero != NULL)
		{	
			cout <<"En el If: " <<numero <<endl;
			suma = suma + atoi(numero);
			n++;
			cout <<"suma: " <<suma <<endl;
		}
		numero = strtok (NULL, "-");	
	}
	promedio = suma/(n-1);	
	mensaje = "Promedio: " + doubletostring(promedio);
	cout <<mensaje <<endl;
	return 0;
}



//Convierte de doble a cadena
string doubletostring(double x)
{
	stringstream ss;
	ss << x;
	string cadena = ss.str();
	return cadena;
}

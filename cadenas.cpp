#include "iostream"
#include "stdio.h"
#include "string.h"

using namespace std;

#define MAX 100

int main()
{
	char cad[MAX];
	string scad;
	cout<<"Cadena separa por comas...\n";
	scad = "Juan,Ana,Luisa,Andres";
	cout<<"Cadena compuesta: " <<scad<<endl;
	strcpy(cad, scad.c_str());
	char *palabra;
	palabra = strtok (cad, ",");
	while(palabra != NULL)
	{
		cout << "Palabra: " <<palabra <<endl;
		palabra = strtok (NULL, ",");
	}
	return 0;
}

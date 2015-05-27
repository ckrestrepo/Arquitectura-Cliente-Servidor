#include "stdio.h"
#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "sstream"
#include "wait.h"
#include "fstream"
#include "list"

#define Archivo "ejemplo.txt"
#define MAXLONG 256


using namespace std;

int main()
{
	ofstream limpiar;
    limpiar.open(Archivo, ios::trunc);
    limpiar <<"Esto no tiene nada\n";
    limpiar.close();

	FILE *fp;
	fp = fopen(Archivo, "a");
	fputs("Nombre apellido otro \n", fp);
	fclose(fp);

	FILE *archivo;
	char caracteres[MAXLONG];
	list<string> l_usuarios;

	archivo = fopen(Archivo, "r");
	if (archivo == NULL)
	{
		exit(1);
	}
	
	cout<<"Extrayendo lista de usuarios..." <<endl;
	while(feof(archivo) == 0)
	{
		fgets(caracteres, MAXLONG, archivo);
		string info = (string)caracteres;
		cout <<"Contenido del archivo: "<<info <<endl;
		l_usuarios.push_back(info);
	}
	fclose(archivo);	
}
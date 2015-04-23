#include "stdio.h"
#include "stdlib.h"
#define archivo "ejemplo.txt"

int main()
{
	FILE *fp;
	fp = fopen(archivo, "a");
	fputs("Nombre apellido otro \n", fp);
	fclose(fp);
	
}
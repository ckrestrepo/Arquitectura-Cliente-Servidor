#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"

using namespace std;

int main()
{
	cout <<"\nAlgo\n" <<endl;
	string a, b, cadena;
	cout <<"Digite el primer numero: ";
	cin >> a;
	cout <<"Digite el segundo numero: ";
	cin >> b;
	cadena = "./proargumento " + a + " " + b;
	char cad [128];
	strcpy(cad, cadena.c_str());
	system(cad);
	return (0);
}

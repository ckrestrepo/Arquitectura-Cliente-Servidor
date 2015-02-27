#include "iostream"
#include "list"


using namespace std;

//Creacion de estructuras

struct persona
{
	string nombre;
	string apellido;
};

void mostrar(list<int>);
list<int> leer();
void mostrarp(list<persona>);
list<persona> leerp();
list<persona> eliminar(list<persona>);

int main()
{
	persona p;
	list <persona> ppl;
	ppl = leerp();
	mostrarp(ppl);	
	eliminar(ppl);
	mostrarp(ppl);
}

// PARA PERSONAS

void mostrarp(list<persona> per)
{
	cout <<"Cantidad de personas: " <<per.size() <<endl;
	list <persona>:: iterator i;
	for (i=per.begin(); i!= per.end(); i++)
	{
		persona p;
		p =*i;
		cout <<"Nombre: "<< p.nombre<< endl;
		cout <<"Apellido: "<< p.apellido <<endl;
	}	
}

// Agregar personas
list<persona> leerp()
{
	persona p;
	int opcion;
	list <persona> per;
	do
	{
		cout <<"Digite el nombre: ";
		cin >> p.nombre;
		cout <<"Digite el apellido: ";
		cin >> p.apellido;
		per.push_back(p);
		cout <<"Desea ingresar mas personas... <Si=1 - No=0: ";
		cin >> opcion;
	}while(opcion != 0);
	return per;
}

//Eliminar personas
list<persona> eliminar(list<persona> per)
{
	string nombre, apellido;
	persona p;
	cout <<"Digite el nombre: ";
	cin >> p.nombre;
	cout <<"Digite el apellido: ";
	cin >> p.nombre;
	nombre = p.nombre;
	apellido = p.nombre;
	per.remove_if((p.nombre == nombre)&&(p.apellido == apellido));
	return per;
}





// PARA ENTEROS

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

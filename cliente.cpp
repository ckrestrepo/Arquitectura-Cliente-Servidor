/* ..... CLIENTE ....*/

#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "unistd.h"
#include "sstream"

using namespace std;

#define MIPUERTO 9091
#define MAXLONGITUD 128

int nosocket, numbytes;
char msj[MAXLONGITUD];
char const host_servidor[]="127.0.0.1";
struct hostent *host_entrante;
struct sockaddr_in servidor;

// Funciones declaradas
int iniciar();
void conectar();
string recibir(int);
void enviar(int, string);
int stringtoint(string);
string inttostring(int);

int main()
{	
	iniciar();
	conectar();
	cout<<"Mensaje: " <<recibir(nosocket) <<endl;
	//Ciclo del servicio
	int opcion, a, b;
	do
	{
		do
		{
			cout <<"\n...Menu de Opciones...\n";
			cout <<"1. Sumar\n";
			cout <<"2. Contar caracteres\n";
			cout <<"3. Salir...\n";
			cout <<"Digite una opcion: ";
			cin >> opcion;
		}while (opcion < 0 || opcion > 3);
		enviar(nosocket, inttostring(opcion));
		cout <<recibir(nosocket)<<endl;
	}while(opcion != 3);

	/*
	string msjenv;
	msj[0] ='\0';
	cout <<"Digite cualquier cosa: ";
	cin >> msjenv;
	strcpy(msj, msjenv.c_str()); // c_str() convierte a char
	send(nosocket, msj, msjenv.size(), 0);
	*/
	close(nosocket);
}



//Metodos
int iniciar()
{
	host_entrante = gethostbyname(host_servidor);
	if (host_entrante == NULL)
	{
		// llamada a gethostbyname()
		cout << "Error al recibir host" <<endl;
		exit(-1);
	}
	
	nosocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nosocket == -1)
	{
		cout <<"Error en conexion 1" <<endl;
		exit(-1);
	}
	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(MIPUERTO);
	servidor.sin_addr = *((struct in_addr *)host_entrante ->h_addr);
	//he pasa la informacion de *he hacia h_addr
	bzero(&(servidor.sin_zero), 8);
}

void conectar()
{
	if (connect(nosocket, (struct sockaddr *)&servidor, sizeof(struct sockaddr)) == -1)
	{
		cout <<"Error en conexion 2" <<endl;
		exit(-1);
	}
}

void enviar (int sock, string cadena)
{
    char mensaje[MAXLONGITUD];
    strcpy(mensaje, cadena.c_str());
    send(sock, mensaje, cadena.size(), 0);
}

string recibir (int sock)
{
    char msj[MAXLONGITUD];
    string mensaje = ""; //toca inicializarlo en vacio
    numbytes = recv (sock, msj, MAXLONGITUD, 0);
    if (numbytes == -1)
    {
        cout <<"Error al recibir. " <<endl;
    }
    if (numbytes == 0)
    {
        cout <<"Conexion finalizada" <<endl;
    }
    if (numbytes > 0)
    {
        msj[numbytes] = '\0'; //con el backslash cero indica que hasta ahi llega el mensaje
        mensaje = (string)msj;
    }
    return mensaje;
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
	stringstream ss;
	ss<<x;
	string cadena = ss.str();
	return cadena;
}


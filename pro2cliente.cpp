/* ..... CLIENTE CON PROCESOS....*/

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
#include "wait.h"

using namespace std;

#define MIPUERTO 9090
#define MAXLONG 256

struct servicios
{
	string idservicio;
	string info;
};

servicios ser;

int nosocket, numbytes;
string mensaje;
char msj[MAXLONG];
char const host_servidor[]="192.168.8.79";
struct hostent *host_entrante;
struct sockaddr_in servidor;

//Funciones declaradas
int iniciar();
void conectar();
string recibir(int);
void enviar(int, string);
void Enviar (int, string, string);
ser Recibir (int);



int main()
{
	iniciar();
	conectar();
	int env;
	mensaje = recibir(nosocket);
	cout <<mensaje <<endl;
	//Gestion de servicio
	int op = 0;
	string ms;
	do
	{
		cout <<"Digite el mensaje: ";
		cin >> ms;
		//enviar(nosocket, ms);
		Enviar (nosocket, "31.Servicio 1\n");
		ser = Recibir(nosocket);
		cout <<"ID: "<<ser.idservicio <<"Info: " <<ser.info <<endl;
		//recibir(nosocket);
	}while(ms != "fin");
	enviar(nosocket, "2,fin.\n");
	close(nosocket);
}

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
    char mensaje[MAXLONG];
    strcpy(mensaje, cadena.c_str());
    send(sock, mensaje, cadena.size(), 0);
}

void Enviar (int ns, string idservicio, string infoservicio)
{
	char msjenv[MAXLONG];
	string mb = idservicio + "," + infoservicio;
	strcpy(msjenv, mbc_str());
	send(ns, msjenv, mb.size(), 0);
}

string recibir (int sock)
{
    char msj[MAXLONG];
    string mensaje = ""; //toca inicializarlo en vacio
    numbytes = recv (sock, msj, MAXLONG, 0);
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

ser Recibir (int ns)
{
	ser infoservicio;
	int nb, salida;
	char msj[MAXLONG];
	nb = recv(ns, msj, MAXLONG, 0);
	if(nb == -1)
	{
		cout <<"Error al recibir.\n";
		infoservicio.idservicio = "10";
		infoservicio.info = "Error al recibir";
	}
	if (nb == 0)
	{
		cout <<"Conexion finalizada\n";
		close(ns);
		infoservicio.idservicio = "4";
		infoservicio.info = "Conexion finalizada sin problemas";
	}
	if (nb > 0)
	{
		msj[nb] = '\0';
		cout <<"Mensaje del cliente [" <<ns <<"]: " <<msj <<endl;
		char datos[MAXLONG];
		string info, id;
		strcpy(datos,msj);		
		msj[0] = '\0';
		char *palabra;
		palabra = strtok (datos, ",");
		id = (string)palabra;
		palabra = strtok (NULL, ",");
		info = (string)palabra;
		infoservicio.idservicio = id;
		infoservicio.info = info;		
	}
	return infoservicio;
}
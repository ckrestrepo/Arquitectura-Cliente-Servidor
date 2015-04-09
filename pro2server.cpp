/* ..... SERVIDOR CON PROCESOS ....*/

#include "stdio.h"
#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sstream"
#include "wait.h"
#include "list"

using namespace std;

#define MIPUERTO 9090   //pto de conexion para usuarios
#define MIIP "192.168.8.79"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONG 256

//Estructuras
struct usuarios
{
	string user;
	string ip;
	int idint;
};

struct servicios
{
	string idservicio;
	string info;
};

list<usuarios> users;
usuarios us;
servicios ser;

//Definicion de Funciones
int inicializar();
//int Recibir (int);
ser Recibir(int);
void Responder (int);
void Enviarinicial (int, struct sockaddr_in);
void Enviar (int, string, string);
int nueva_cx (int);
void mostrar(list<usuarios>);


//Variables "globales"
int nosocket, nuevo;
struct sockaddr_in dirservidor;
struct sockaddr_in cliente;
socklen_t lon;

int main()
{
	inicializar();
    cout <<"\n...Servidor Iniciado...\n";
	int fin = 1;
	do
	{
		nuevo = nueva_cx(nosocket);
		pid_t idProceso;
		int estadoHijo;
		idProceso = fork();
		if(idProceso == -1)
		{
			perror ("No se puede crear el proceso\n");
			exit(-1);
		}
		//Hijo
		if (idProceso == 0)
		{
			//Ciclo de servicio del Hijo
			do
			{
				ser.idservicio = Recibir(nuevo);
				//31: "Servicio 1"
				if(ser.idservicio == "31")
				{
					Servicio1(nuevo);
				}
			}while (ser.idservicio != "2");
			exit (33);
		}
		//Padre
		if(idProceso > 0)
		{
			Enviarinicial(nuevo, cliente);
			us.user = "vacio";
			char *dire;
			dire = inet_ntoa(cliente.sin_addr);
			us.ip = (string)dire;
			us.idint = nuevo;
			users.push_back(us);
			mostrar(users);
		}
	}while (fin == 1);
	return 0;
}

void servicio1(int ns)
{
	//Solicitar lista al padre mediante tuberia
	sitrng info, mensaje;
	list<usuarios>::iterator i;
	for (i = ns.begin(); i != ns.end(); i++)
	{
		us = *i;
		mensaje = aCad(us.idint) + ":" + us.user;
		Enviar(ns, "30", mensaje);
		ser = Recibir(ns);
		cout <<ser.info<<endl;
	}
	Enviar(ns, "3", "fin");
	ser = Recibir(ns);
	cout <<ser.info <<endl;
}

void mostrar(list<usuarios> usr)
{
	int registro = 1;
	cout <<"Cantidad de Usuarios: " <<usr.size() <<endl;
	list <usuarios>:: iterator i = usr.begin();
	while (i!= usr.end())
	{
		//usuarios us este es el global
		us =*i;
		cout <<"\nRegistro: " << registro <<endl;;
		cout <<"Usuario: "<< us.user<< endl;
		cout <<"ID interno: "<< us.idint <<endl;
		cout <<"Direccion IP: "<< us.ip <<endl;
		i++;
		registro++;
	}	
}

int inicializar ()
{
    nosocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nosocket == -1)
    {
        cout <<"Error de Socket..."<<endl;
        exit(-1);
    }
    dirservidor.sin_family = AF_INET;
    dirservidor.sin_port = htons(MIPUERTO);
    dirservidor.sin_addr.s_addr = inet_addr("192.168.8.79");
    bzero(&(dirservidor.sin_zero),8);
    if (bind(nosocket, (struct sockaddr*)&dirservidor, sizeof(struct sockaddr))==-1)
    {
        cout <<"Error en bind()" <<endl;
        exit(-1);
    }
    if (listen(nosocket,ENCOLA) == -1)
    {
        cout <<"Error en listen()" <<endl;
        exit(-1);
    }
}

// Funcion que hace una nueva conexion
int nueva_cx (int sock)
{
    lon = sizeof(struct sockaddr_in);
    int ns = accept(sock, (struct sockaddr*)&cliente, &lon);
    if (nuevo == -1)
    {
        cout <<"error en accept()" << endl;
        exit(-1);
    }
    return ns;
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
		datos = msj;
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

void Responder (int ns)
{
    send(ns, "1,Recibido", 10, 0);
}

void Enviarinicial (int ns, struct sockaddr_in cl)
{
	char *dire;
	dire = inet_ntoa(cl.sin_addr);
	send(ns, "\n30,....Bienvenido al Servidor ...\n", 38, 0);
	cout <<"Direccion entrante: "<<dire <<endl;
}

void Enviar (int ns, string idservicio, string infoservicio)
{
	char msjenv[MAXLONG];
	string mb = idservicio + "," + infoservicio;
	strcpy(msjenv, mbc_str());
	send(ns, msjenv, mb.size(), 0);
}

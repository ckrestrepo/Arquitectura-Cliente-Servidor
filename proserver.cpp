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

using namespace std;

#define MIPUERTO 9090   //pto de conexion para usuarios
#define MIIP "192.168.8.79"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONG 256

//Definicion de Funciones
int inicializar();
int Recibir (int);
void Responder (int);
void Enviar (int, struct sockaddr_in);
int nueva_cx (int);


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
			int estado = 0;
			//Ciclo de servicio del Hijo
			do
			{
				estado = Recibir(nuevo);
				if(estado == 1)
				{
					Responder(nuevo);
				}
			}while ((estado != 33) && (estado != -1));
			exit (33);
		}
		if(idProceso > 0)
		{
			Enviar(nuevo, cliente);
		}
	}while (fin == 1);
	return 0;
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

int Recibir (int ns)
{
	int nb, salida;
	char msj[MAXLONG];
	nb = recv(ns, msj, MAXLONG, 0);
	if(nb == -1)
	{
		cout <<"Error al recibir.\n";
		salida = -1;
	}
	if (nb == 0)
	{
		cout <<"Conexion finalizada\n";
		close(ns);
		salida = 33;
	}
	if (nb > 0)
	{
		msj[nb] = '\0';
		cout <<"Mensaje del cliente [" <<ns <<"]: " <<msj <<endl;
		msj[0] = '\0';
		salida = 1;
	}
	return salida;
}

void Responder (int ns)
{
    send(ns, "Breve la vuelta...\n", 19, 0);
}

void Enviar (int ns, struct sockaddr_in cl)
{
	char *dire;
	dire = inet_ntoa(cl.sin_addr);
	send(ns, "\n....Bienvenido al Servidor ...\n", 34, 0);
	cout <<"Direccion entrante: "<<dire <<endl;
}


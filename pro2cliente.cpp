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

#define MIPUERTO 9093
#define MAXLONG 256

typedef struct servicios
{
	string idservicio;
	string info;
}ds;

int nosocket, numbytes, opcion;
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
ds Recibir (int);
void Servicio1(int);
void mostrar_menu();



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
		mostrar_menu();
		switch (opcion)
		{
			case 1:
			{
				enviar (nosocket, "31,Servicio 1\n");
				ds info = Recibir(nosocket);
				cout <<"ID: "<<info.idservicio <<endl <<"Info: " <<info.info <<endl;
				//Recibir informacion
				Servicio1(nosocket);
			}
				break;
			case 2:
			{
				enviar (nosocket, "32,Servicio 2\n");
				ds info = Recibir(nosocket);
				cout <<"ID: "<<info.idservicio <<endl <<"Info: " <<info.info <<endl;
				Servicio1(nosocket);
			}
				break;
			case 3:
			{
				enviar (nosocket, "33,Servicio 3\n");
				ds info = Recibir(nosocket);
				cout <<"ID: "<<info.idservicio <<endl <<"Info: " <<info.info <<endl;
				Servicio1(nosocket);
			}
				break;
			case 0:
				break;
			default:
				cout <<"Opcion incorrecta... Imbecil\n";
				break;
		}

	}while(opcion != 0);
	enviar(nosocket, "2,fin.\n");
	close(nosocket);
}

void Servicio1(int ns)
{
	cout <<"Iniciando el Servicio 1...\n";
	ds informacion;
	informacion = Recibir(nosocket);
	cout <<"ID: "<<informacion.idservicio <<endl <<"Info: " <<informacion.info <<endl;
	enviar (nosocket, "1, Recibido\n");
	do
	{
		informacion = Recibir(ns);
		cout <<"ID: "<<informacion.idservicio <<endl <<"Info: " <<informacion.info <<endl;
		Enviar(ns, "1", "Recibido");
	}while (informacion.idservicio != "3");
}

// MENUS
void mostrar_menu()
{
	cout <<"\n...Menu Principal...\n";
	cout <<"1. Listar Jugadores\n";
	cout <<"2. Registrarse\n";
	cout <<"3. Jugar\n";
	cout <<"0. Salir...\n";
	cout <<"Digite una opcion: ";
	cin >> opcion;
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

string enviarInfo (int s, string campo){
   char msjenv[MAXLONG];  
   cout<<"Enviar "<< campo <<":" ;
   cin.getline(msjenv,MAXLONG,'\n');
   mensaje=(string) msjenv;
   send(s,msjenv,mensaje.size(),0);
   return mensaje;
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
	strcpy(msjenv, mb.c_str());
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

string recibir (int s, int vc)
{
	int nb;
  	string msjnuevo="vacio";
	char msjrec[MAXLONG];
	nb = recv(s,msjrec,MAXLONG,0);
	if (nb == -1)
	{
	cout<<"Error al recibir. "<< endl;
	exit(-1);
	}
	if (nb==0)
	{
	cout<<"Conexion finalizada"<<endl;
	close(s);
	}
	if(nb>0)
	{
	msjrec[nb]='\0';
	//cout<<"Mensaje: "<<msjrec<<endl;
	msjnuevo=(string)msjrec;
	}
	return msjnuevo;
}

ds Recibir (int ns)
{
	ds infoservicio;
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
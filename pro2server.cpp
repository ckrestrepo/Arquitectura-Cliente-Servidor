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

#define MIPUERTO 9093   //pto de conexion para usuarios
#define MIIP "192.168.8.79"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONG 256
#define LISTADO "lista.usr.srv"
//#define archivo "ejemplo.txt"


//Estructuras
typedef struct usuarios
{
	string user;
	string ip;
	int idint;
}users;

typedef struct servicios
{
	string idservicio;
	string info;
}ds;

list<users> l_us;

//Definicion de Funciones
int inicializar();
//int Recibir (int);
ds Recibir(int);
void Responder (int);
void Enviarinicial (int, struct sockaddr_in);
void Enviar (int, string, string);
int nueva_cx (int);
void mostrar(list<usuarios>);
void Buscar(list<string>);
list<string> Abrir();
void Servicio1(int);
void Servicio2(int);
void Servicio3(int);
void Guardar(users);
string inttostring(int);
users stringtoUsers(string);

struct sockaddr_in dirservidor;
struct sockaddr_in cliente;

//Variables "globales"
int nosocket, nuevo;
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
			ds iserv;
			//Ciclo de servicio del Hijo
			do
			{
				iserv = Recibir(nuevo);
				Enviar(nuevo, "1", "Opcion recibida");
				//31: "Servicio 1"
				cout <<"Opcion ID: "<<iserv.idservicio <<endl <<"Info: " <<iserv.info <<endl;
				if(iserv.idservicio == "31")
				{
					// Lista de usuarios
					Servicio1(nuevo);
				}
				if(iserv.idservicio == "32")
				{
					// Registrar usuario
					Servicio2(nuevo);
				}
				if(iserv.idservicio == "33")
				{
					// Jugar
					Servicio3(nuevo);
				}
			}while (iserv.idservicio != "2");
			exit (33);
		}
		//Padre
		if(idProceso > 0)
		{
			Enviarinicial(nuevo, cliente);
			users us;
			us.user = "vacio";
			char *dire;
			dire = inet_ntoa(cliente.sin_addr);
			us.ip = (string)dire;
			us.idint = nuevo;
			l_us.push_back(us);
			Guardar(us);
			mostrar(l_us);
		}
	}while (fin == 1);
	return 0;
}

void Servicio1(int ns)
{
	//Solicitar lista al padre mediante tuberias nombrada
	//string info;
	ds iserv;
	//Inicia con enviar
	Enviar(ns, "30","Extrayendo...");
	iserv = Recibir(ns);
	cout <<"ID: "<<iserv.idservicio <<endl <<"Info: " <<iserv.info <<endl;
	list<string> l;
	l = Abrir();

	list<string>:: iterator i;
	for (i = l.begin(); i != l.end(); i++)
	{
		string usr;
		usr = *i;
		Enviar(ns, "1", usr);
		iserv = Recibir(ns);
	}
	Enviar(ns, "3", "Fin del proceso...\n");
	iserv = Recibir(ns);

}

void Servicio2(int ns)
{
	// Registro de usuarios
	ds iserv = Recibir(ns);
	cout <<"ID: "<<iserv.idservicio <<endl <<"Info: " <<iserv.info <<endl;
	Enviar(nuevo, "1", "Opcion recibida");

	// 1. Extraer informacion del archivo a una lista
	list<string> l_usuarios;
	l_usuarios = Abrir();
	// 2. Buscar coincidencia de id socket en la lista
	Buscar(l_usuarios);
	// 3. Verificar nombre (que no se repita)

	// 4. Si no se repite, cambiar valor en la lista

	// 5.

}

void Servicio3(int ns)
{
	string info;
	Enviar(ns, "30","Servicio3 Recibido...");
}

void Buscar(list<string> l)
{
	list <string>:: iterator i;
	for (i = l.begin(); i!= l.end(); i++)
	{
		string cadena;
		cadena = *i;
		users infousr = stringtoUsers(cadena);
		cout <<"Valores...\n";
		cout <<"Usuario: "<<infousr.user <<endl;
		cout <<"ID interno: " <<infousr.idint <<endl;
	}
}



void mostrar(list<users> usr)
{
	int registro = 1;
	cout <<"Cantidad de Usuarios: " <<usr.size() <<endl;
	list <users>:: iterator i = usr.begin();
	while (i!= usr.end())
	{
		users us;
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

void Responder (int ns)
{
    send(ns, "1,Recibido", 10, 0);
}

void Enviarinicial (int ns, struct sockaddr_in cl)
{
	char *dire;
	dire = inet_ntoa(cl.sin_addr);
	string mb = "30,...Bienvenido al Servidor...\n";
	char msjenv[MAXLONG];
	strcpy(msjenv, mb.c_str());
	send(ns, msjenv, mb.size(),0);
	cout <<"Direccion entrante: "<<dire <<endl;
}

void Enviar (int ns, string idservicio, string infoservicio)
{
	char msjenv[MAXLONG];
	string mb = idservicio + "," + infoservicio;
	strcpy(msjenv, mb.c_str());
	send(ns, msjenv, mb.size(), 0);
}

list<string> Abrir()
{
	FILE *archivo;
	char caracteres[MAXLONG];
	list<string> l_usuarios;

	archivo = fopen(LISTADO, "r");
	if (archivo == NULL)
	{
		exit(1);
	}
	
	cout<<"Extrayendo lista de usuarios..." <<endl;
	while(feof(archivo) == 0)
	{
		fgets(caracteres, MAXLONG, archivo);
		string info = (string)caracteres;
		//printf("%s",caracteres);
		cout <<info <<endl;
		l_usuarios.push_back(info);
	}
	fclose(archivo);
	cout <<endl;
	return l_usuarios;
}

void Guardar(users u)
{
	FILE *fp;
	fp = fopen(LISTADO, "a");
	char registro[MAXLONG];
	string sreg = u.user + "," + u.ip + "," + inttostring(u.idint) + "\n";
	strcpy(registro, sreg.c_str());
	fputs(registro, fp);
	fclose(fp);

}

string inttostring(int x)
{
    stringstream ss;
    ss<<x;
    string cadena = ss.str();
    return cadena;
}

users stringtoUsers(string cad)
{
	/*
	string user;
	string ip;
	int idint;
	*/
	users infousr;
	char datos[MAXLONG];
	strcpy(datos, cad.c_str());
	string user, ip, idint;
	char *ptr = strtok(datos,",");
	user = (string)ptr;
	ptr = strtok (NULL, ",");
	ip = (string)ptr;
	ptr = strtok (NULL, ",");
	idint = (string)ptr;
	infousr.user = user;
	infousr.ip = ip;
	infousr.idint = stringtoUsers(idint);

	return infousr;

}
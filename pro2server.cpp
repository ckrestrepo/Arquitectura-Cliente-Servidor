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
#include "fstream"
#include "list"

using namespace std;

#define MIPUERTO 9093   //pto de conexion para usuarios
#define MIIP "127.0.0.1"
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
void mostrar(list<users>);

bool BuscarRepetido(list<string>, int);
void Buscar(list<string>, int, string);
list<string> Abrir();
void Servicio1(int);
void Servicio2(int);
void Servicio3(int);
void Guardar(users, int);
string inttostring(int);
int stringtoint(string);
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
    ofstream limpiar;
    limpiar.open(LISTADO, ios::trunc);
    limpiar <<"";
    limpiar.close();
	int fin = 1;
	bool primero = true;
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
				cout <<"Opcion ID: "<<iserv.idservicio <<endl;
				cout <<"Info: " <<iserv.info <<endl;
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
			bool serepite;
			Enviarinicial(nuevo, cliente);
			users us;
			us.user = "vacio";
			char *dire;
			dire = inet_ntoa(cliente.sin_addr);
			us.ip = (string)dire;
			us.idint = nuevo;

			list<string> l_usuarios;
			l_usuarios = Abrir();
			cout <<"Despues de abrir\n";
			serepite = BuscarRepetido(l_usuarios, nuevo);
			cout <<"Despues de Buscar Repetido\n";
			if (serepite != true)
			{
				cout <<"Entral al if del repite\n";
				l_us.push_back(us);
				//Almacenamos la informacion del usuario en Archivo
				cout <<"Guardando...\n";
				if (primero)
				{
					Guardar(us, 0);
					primero = false;
				}
				else
				{
					Guardar(us, 1);
				}
			}			
			mostrar(l_us);
		}
	}while (fin == 1);
	return 0;
}

void Servicio1(int ns)
{
	//Solicitar lista al padre mediante tuberias nombrada
	ds iserv;
	//Inicia con enviar
	Enviar(ns, "30","Extrayendo...");
	iserv = Recibir(ns);
	cout <<"ID: "<<iserv.idservicio <<endl;
	cout <<"Info: " <<iserv.info <<endl;
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
	ds iserv;
	iserv = Recibir(ns);
	Enviar(nuevo, "1", "Opcion recibida");
	cout <<"ID: "<<iserv.idservicio <<endl; 
	cout <<"Info: " <<iserv.info <<endl;
	string nombre = iserv.info;
	// 1. Extraer informacion del archivo a una lista
	list<string> l_usuarios;
	l_usuarios = Abrir();
	// 2. Buscar coincidencia de id socket en la lista
	// 3. Verificar nombre (que no se repita)
	// 4. Si no se repite, cambiar valor en la lista
	// 5. Reemplazar datos del archivo por informacion en la lista
	Buscar(l_usuarios, ns, nombre);
}

void Servicio3(int ns)
{
	string info;
	Enviar(ns, "30","Servicio3 Recibido...");
}


bool BuscarRepetido(list<string> l, int idint)
{
	list <string>:: iterator i;
	bool repetido = false;
	for (i = l.begin(); i!= l.end(); i++)
	{
		string cadena;
		cadena = *i;
		if (cadena != "0")
		{
			users infousr = stringtoUsers(cadena);
			cout <<cadena;
			if (idint == infousr.idint)
			{
				repetido = true;
			}
		}
	}
	return repetido;
}

void Buscar(list<string> l, int idser, string nombre)
{
	list <string>:: iterator i;
	bool repetido = false;
	// Aca miramos si se repite el nombre
	for (i = l.begin(); i!= l.end(); i++)
	{
		string cadena;
		cadena = *i;
		users infousr = stringtoUsers(cadena);
		if (nombre == infousr.user)
		{
			repetido = true;
		}
		cout <<"Valores...\n";
		cout <<"Usuario: "<<infousr.user <<endl;
		cout <<"ID interno: " <<infousr.idint <<endl;
	}
	list<users> nuevalista;
	if(!repetido)
	{
		list<string>::iterator i2;
		for (i2 = l.begin(); i2!= l.end(); i2++)
		{
			string cadena;
			cadena = *i2;
			users infousr = stringtoUsers(cadena);
			if(idser == infousr.idint)
			{
				infousr.user = nombre;
			}
			nuevalista.push_back(infousr);
		}
		//reescribir archivo lista.usr.srv
		list<users>::iterator i_users;
		bool primero = true;
		for (i_users = nuevalista.begin(); i_users!= nuevalista.end(); i_users++)
		{
			users aux;
			aux = *i_users;
			if (primero)
			{
				Guardar(aux,0);
				primero = false;
			}
			else
			{
				Guardar(aux,1);
			}
		}
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
		cout <<info <<endl;
		l_usuarios.push_back(info);
	}
	fclose(archivo);
	return l_usuarios;
}

// Tipo 1: Adicion, Tipo 0: Nuevo
void Guardar(users u, int tipo)
{
	FILE *fp;
	if (tipo == 1)
	{
		fp = fopen(LISTADO, "a");
	}
	else
	{
		fp = fopen(LISTADO, "w");	
	}
	
	char registro[MAXLONG];
	string sreg = u.user + "," + u.ip + "," + inttostring(u.idint) + "\n";
	strcpy(registro, sreg.c_str());
	fputs(registro, fp);
	fclose(fp);
	cout <<"Guardado con Exito...\n";
}

int stringtoint(string x)
{
    int valor = atoi(x.c_str());
    return valor;
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
	infousr.idint = stringtoint(idint);
	return infousr;
}



/****************************************************/
/*               SECCION DE CONEXION                */
/****************************************************/


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
    dirservidor.sin_addr.s_addr = inet_addr("127.0.0.1");
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
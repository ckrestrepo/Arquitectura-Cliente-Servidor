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

int nosocket, numbytes, opcion, opmenu2;
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
void mostrar_menu();
void menu2();

int main()
{	
	iniciar();
	conectar();
	cout<<"Mensaje: " <<recibir(nosocket) <<endl; // Recibido numero (1 )
	//Ciclo del servicio
	string nombre, usuario, pw1, pw2;
	do
	{
		mostrar_menu();
		enviar(nosocket, inttostring(opcion));	// Enviar opcion (1)
		cout << recibir(nosocket) <<endl;		// Recibido de la opcion enviada (2)
		switch(opcion)
		{
			case 1: 
				cout <<"\n...AGREGAR REGISTRO...\n";
				cout <<"Digite el nombre: ";
				cin >> nombre;
				enviar (nosocket, nombre);
				cout <<recibir(nosocket) <<endl;
				cout <<"Digite su ID: ";
				cin >> usuario;
				enviar (nosocket, usuario);
				cout <<recibir(nosocket) <<endl;
				cout <<"Digite la contraseña: ";
				cin >> pw1;
				cout <<"Vuelva a escribir la contraseña: ";
				cin >> pw2;
				while(pw1 != pw2)
				{
					cout <<"Digite la contraseña: ";
					cin >> pw1;
					cout <<"Vuelva a escribir la contraseña: ";
					cin >> pw2;
				}
				enviar (nosocket, pw2);
				cout <<recibir(nosocket) <<endl;
				break;
			case 2:
				cout <<"Digite su ID: ";
				cin >> usuario;
				enviar (nosocket, usuario);
				cout <<recibir(nosocket) <<endl;
				cout <<"Digite la contraseña: ";
				cin >> pw1;
				enviar (nosocket, pw1);
				cout <<recibir(nosocket) <<endl;
				enviar (nosocket, "Estoy registrado?...\n");
				cout <<recibir(nosocket)<<endl;
				break;
			case 3:
				do
				{
					menu2();
					string menrec="";
					enviar(nosocket, inttostring(opmenu2));
					cout << recibir(nosocket) <<endl;
					cout <<"\nDigite su ID para comprobar que esta registrado: ";
					cin >> usuario;
					enviar (nosocket, usuario);
					menrec = recibir(nosocket);
					cout << menrec <<endl;
					switch(opmenu2)
					{
						case 1:
							if (menrec == "Usuario Existe")
							{
								cout <<"Digite nuevo nombre: ";
								cin >> nombre;
								enviar(nosocket, nombre);
								cout <<recibir(nosocket) <<endl; // Debe de decir nombre modificado
							}
							else
							{
								
							}
							break;
						case 2:
							break;
						case 3:
							break;
						case 0:
							enviar(nosocket, "\nHe salido del servidor...\n");
							cout << recibir(nosocket) <<endl;
							break;
						default:
							cout <<"Opcion incorrecta... Imbecil\n";
							break;				
					}
				}while(opmenu2 != 0);
				break;
			case 0:
				enviar(nosocket, "\nHe salido del servidor...\n");
				cout << recibir(nosocket) <<endl;
				break;
			default:
				cout <<"Opcion incorrecta... Imbecil\n";
				break;
		}
	}while(opcion != 0);
	close(nosocket);
}

void menu2()
{
	cout <<"\n...Datos a modificar...\n";
	cout <<"1. Modificar Nombre\n";
	cout <<"2. Modificar Usuario\n";
	cout <<"3. Modificar Contraseña\n";
	cout <<"0. Salir...\n";
	cout <<"Digite una opcion: ";
	cin >> opmenu2;
}
void mostrar_menu()
{
	cout <<"\n...Menu de Opciones...\n";
	cout <<"1. Registrarse\n";
	cout <<"2. Autenticarse\n";
	cout <<"3. Modificar registro\n";
	cout <<"0. Salir...\n";
	cout <<"Digite una opcion: ";
	cin >> opcion;
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


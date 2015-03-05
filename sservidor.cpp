/* ..... SERVIDOR ....*/
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
#include "list"

using namespace std;

#define MIPUERTO 9091   //pto de conexion para usuarios
#define MIIP "127.0.0.1"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONGITUD 128

int inicializar();
void verIP(struct sockaddr_in, int);
int nueva_cx(int);
string recibir(int);
void enviar(int);
void enviar(int, string);
int stringtoint(string);
double stringtodouble(string);
string inttostring(int);
string doubletostring(double);


void servicio1();
void servicio2();


int nosocket, nuevo_socket;
struct persona
{	
	string nombre;
	string usuario;
	string  pw;
	char* dirIP;
};

bool autenticarR(list<persona>, string, string);
list <persona> ppl;
struct sockaddr_in dirservidor;
struct sockaddr_in cliente;
socklen_t senal_tam;


int main ()
{
    inicializar();
    cout <<"\n...Bienvenido al servidor...\n";
    while(1)
    {
        nuevo_socket = nueva_cx(nosocket);
        enviar(nuevo_socket, "...Bienvenido al servidor...");
        int opcion;
        do
        {
	        cout <<"Cantidad de registros: " <<ppl.size() <<endl;
            string valoropc = recibir(nuevo_socket);		// Recibir opcion del cliente (1)
            cout <<"Opcion del cliente: " << valoropc <<endl;
            //Cambiamos la cadena a entero
            opcion = stringtoint(valoropc);
            enviar(nuevo_socket, "Opcion recibida");	// Enviar opcion recibida (2)
            switch(opcion)
            {
            	case 1: 
            		servicio1();
            		break;
            	case 2:
            		servicio2();
            		break;
            	case 0:
            		cout <<recibir(nuevo_socket);
            		enviar(nuevo_socket, "\nGracias por visitar nuestro SERVIDOR\n"); 
            		break;
            	default:
            		cout <<"El cliente digito una opcion incorrecta..\n";
            		break;
            }
        }while(opcion != 0);
        close(nuevo_socket);
    } // fin del while (1)
    return 0;
}

bool autenticarR(list<persona> per, string user, string pass)
{
	//cout <<"Cantidad de registros: " <<per.size() <<endl;
	list <persona>:: iterator i = per.begin();
	while (i!= per.end())
	{
		persona p;
		p =*i;
		if ((p.usuario == user) && (p.pw == pass))
		{
			return true;
		}
		i++;
	}	
}

void servicio1()
{
	persona p;
	p.dirIP = inet_ntoa(cliente.sin_addr);
	p.nombre = recibir(nuevo_socket);	
	cout <<"El cliente dice: " <<p.nombre <<endl;
    enviar(nuevo_socket, "Nombre recibido");
    p.usuario = recibir(nuevo_socket);	
	cout <<"El cliente dice: " <<p.usuario <<endl;
    enviar(nuevo_socket, "Usuario recibido");
    p.pw = recibir(nuevo_socket);	
    cout <<"El cliente dice: " <<p.pw <<endl;
    enviar(nuevo_socket, "Contraseña almacenada\n");
    cout <<"Direccion del cliente: " <<p.dirIP <<endl;
    ppl.push_back(p);
    cout <<"\nRegistro Almacenado en la lista\n";
}

void servicio2()
{
	persona p;
	bool existe;
	p.usuario = recibir(nuevo_socket);	
	cout <<"ID del registro: " <<p.usuario <<endl;
    enviar(nuevo_socket, "ID recibido");
    p.pw = recibir(nuevo_socket);	
    cout <<"Password del cliente: " <<p.pw <<endl;
    enviar(nuevo_socket, "Contraseña Recibida\n");
    existe = autenticarR(ppl, p.usuario, p.pw);
    cout <<recibir(nuevo_socket);
    if (existe == true)
    {
	    enviar(nuevo_socket, "Usuario con acceso\n");
    }
    else
    {
    	enviar(nuevo_socket, "Usuario SIN acceso al sistema\n");
    }
}


// Funcion que inicializa el servidor
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
    senal_tam = sizeof(struct sockaddr_in);
    int ns = accept(sock, (struct sockaddr*)&cliente, &senal_tam);
    if (nuevo_socket == -1)
    {
        cout <<"error en accept()" << endl;
        exit(-1);
    }
    return ns;
}

// Funcion que permite ver la IP del cliente
void verIP (struct sockaddr_in c, int sock)
{
    char*  dire;
    dire = inet_ntoa(c.sin_addr);
    cout <<"Direccion del cliente: " <<dire <<endl;
    enviar(sock, "...Bienvenido al servidor...");
}

// Funcion que envia mensajes al cliente (tiene sobrecarga)
// digamos dos constructores

void enviar (int sock)
{
    send(sock, "Breve la vuelta...\n", 19, 0);
}

void enviar (int sock, string cadena)
{
    char mensaje[MAXLONGITUD];
    strcpy(mensaje, cadena.c_str());
    send(sock, mensaje, cadena.size(), 0);
}

// Funcion que recive los mensajes
string recibir (int sock)
{
    char msj[MAXLONGITUD];
    string mensaje = ""; //toca inicializarlo en vacio
    int numbytes = recv (sock, msj, MAXLONGITUD, 0);
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
        //cout <<"Numero: " <<msj <<endl;
        mensaje = (string)msj;
    }
    return mensaje;
}

// De cadena a Entero
int stringtoint(string x)
{
    int valor = atoi(x.c_str());
    return valor;
}

// De cadena a Doble
double stringtodouble(string x)
{
	double valor = atof(x.c_str());
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

//Convierte de doble a cadena
string doubletostring(double x)
{
	stringstream ss;
	ss << x;
	string cadena = ss.str();
	return cadena;
}


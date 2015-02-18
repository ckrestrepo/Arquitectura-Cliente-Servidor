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

using namespace std;

#define MIPUERTO 9091   //pto de conexion para usuarios
#define MIIP "127.0.0.1"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONGITUD 128

int inicializar();
int comprobar_socket(int);
void verIP(struct sockaddr_in);
int nueva_cx(int);
char* recibir(int);
void enviar(int);
void enviar(int, string);


int nosocket, nuevo_socket;
struct sockaddr_in dirservidor;
struct sockaddr_in cliente;
socklen_t senal_tam;

// Funcion que inicializa el servidor
int inicializar ()
{
	nosocket = socket(AF_INET, SOCK_STREAM, 0);
	comprobar_socket(nosocket);
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
    cout <<"Nosotros: " <<nosocket <<endl;
}

// Funcion que comprueba si el socket esta habilitado
int comprobar_socket(int numsocket)
{
	if (numsocket == -1)
	{
		cout <<"Error de Socket..."<<endl;
		exit(-1);
	}
}

// Funcion que hace una nueva conexion
int nueva_cx (int ns)
{
	senal_tam = sizeof(struct sockaddr_in);
    nuevo_socket = accept(ns, (struct sockaddr*)&cliente, &senal_tam);
    if (nuevo_socket == -1)
    {
        cout <<"error en accept()" << endl;
        exit(-1);
    }
    return ns;
}

// Funcion que permite ver la IP del cliente
void verIP (struct sockaddr_in c)
{
	char*  dire;
	dire = inet_ntoa(c.sin_addr);
	cout <<"Direccion del cliente: " <<dire <<endl;
	//send(nuevo_socket, "...Bienvenido al servidor...\n\n", 32, 0);
}

int main ()
{
	inicializar();
    while(1)
    {
    	nuevo_socket = nueva_cx(nosocket);
    	verIP(cliente);
        //send(nuevo_socket, "...Bienvenido al servidor...\n\n", 32, 0);
     	cout <<"En espera del primer numero... \n";
     	char msj[MAXLONGITUD], msj2[MAXLONGITUD];
     	int numbytes = recv (nuevo_socket, msj, MAXLONGITUD, 0);
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
     		cout <<"Numero 1: " <<msj <<endl;
     	}
	   	send(nuevo_socket, "Numero uno recibido.\n", 32, 0);

	   	// ESPERA DEL NUMERO DOS
	   	cout <<"En espera del segundo numero...\n";

     	numbytes = recv (nuevo_socket, msj2, MAXLONGITUD, 0);
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
     		msj2[numbytes] = '\0'; //con el backslash cero indica que hasta ahi llega el mensaje
     		cout <<"Numero 2: " <<msj2 <<endl;
     	}
     	send(nuevo_socket, "Numero dos recibido.\n", 32, 0);
     	// Aca seria la operacion
     	int res = (atoi(msj) + atoi(msj2));
     	stringstream stream; 
		stream << res; 
     	string palabra = stream.str();
     	string total = "El resultado es: " + palabra;
     	cout << total;
     	
     	
     	strcpy(msj, total.c_str()); // c_str() convierte a char
     	//Envio el resultado
	   	send(nuevo_socket, msj, total.size(), 0);

	   	//send(nuevo_socket, "Resultado", 12, 0);
	   	
	   	
	   	/*   ASI DEBERIA DE QUEDAR
	   	nuevo_socket = nueva_cx(nosocket);
    	verIP(cliente);
    	cout <<en espera del mensaje... "<<endl;
    	cout <<mensaje del cliente: " <<recibir(nuevo_socket) <<endl;
    	close(nuevo_socket);
	   	*/
     	close(nuevo_socket);
    } // fin del while (1)
    return 0;
}

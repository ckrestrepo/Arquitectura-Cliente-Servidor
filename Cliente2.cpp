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

using namespace std;

#define MIPUERTO 9090
#define MAXLONGITUD 128

int main()
{
	int nosocket, numbytes;
	char msj[MAXLONGITUD];
	char const host_servidor[]="192.168.8.79";
	struct hostent *host_entrante;
	struct sockaddr_in servidor;
	
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
	
	//connect(nosocket, (struct sockaddr *)&servidor, sizeof(servidor)) == -1
	if (connect(nosocket, (struct sockaddr *)&servidor, sizeof(struct sockaddr)) == -1)
	{
		cout <<"Error en conexion 2" <<endl;
		exit(-1);
	}
	
	// Recive respuesta del servidor
	numbytes=recv(nosocket, msj, MAXLONGITUD, 0);
	
	// ACA EMPEZARIA LA INICIALIZACION DE LOS DIGITOS
	
	if (numbytes == -1)
	{
		cout <<"Error al recibir" <<endl;
		exit(-1);
	}
	//send(nosocket, "Saludos\n", 22, 0);
	//msj[numbytes] = '\0';
	cout <<"Mensaje: "<<msj <<endl;
	
	//Nuevo mensaje al servidor
	
	string msjenv;
	msj[0] ='\0';
	cout <<"Digite el numero: ";
	cin >> msjenv;
	strcpy(msj, msjenv.c_str()); // c_str() convierte a char
	send(nosocket, msj, msjenv.size(), 0);
	
	numbytes=recv(nosocket, msj, MAXLONGITUD, 0);
	if (numbytes == -1)
	{
		cout <<"Error al recibir" <<endl;
		exit(-1);
	}
	cout <<msj <<endl;
	
	// Otro mensaje
	
	msj[0] ='\0';
	cout <<"Digite el segundo numero: ";
	cin >> msjenv;
	strcpy(msj, msjenv.c_str()); // c_str() convierte a char
	send(nosocket, msj, msjenv.size(), 0);
	
	numbytes=recv(nosocket, msj, MAXLONGITUD, 0);
	if (numbytes == -1)
	{
		cout <<"Error al recibir" <<endl;
		exit(-1);
	}
	cout <<msj <<endl;
	
	// Recibo el resultado
	numbytes=recv(nosocket, msj, MAXLONGITUD, 0);
	if (numbytes == -1)
	{
		cout <<"Error al recibir" <<endl;
		exit(-1);
	}
	cout <<msj <<endl;
	close(nosocket);
}

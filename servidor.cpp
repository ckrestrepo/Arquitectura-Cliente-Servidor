/* ..... SERVIDOR ....*/
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

int suma(int, int);
void servicio1();
void servicio2();
void servicio3();
void servicio4();

int nosocket, nuevo_socket;
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
        verIP(cliente, nuevo_socket);		// Enviar mensaje (1)
        int opcion;
        do
        {
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
            	case 3:
            		servicio3();
            		break;
            	case 4:
            		servicio4();
            		break;
            	case 0:
            		cout <<recibir(nuevo_socket);
            		enviar(nuevo_socket, "\nGracias por visitar nuestro SERVIDOR\n"); //Enviar en caso de que sea la opcion 3
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

void servicio1()
{
	string dato1, dato2, mensaje;
	int resultado;
	cout <<"... Suma de dos digitos ...\n";
	dato1 = recibir(nuevo_socket);					// Recibio digito a (2)
	cout << "Primer digito: " << dato1 <<endl;
	enviar(nuevo_socket, "Numero 1 recibido\n");	// enviar informe de numero recibido (3)
	dato2 = recibir(nuevo_socket);					// Recibio digito b (3)
	cout << "Segundo digito: " << dato2 <<endl;
	enviar(nuevo_socket, "Numero 2 recibido\n");	// enviar informe de numero recibido (4)
	resultado = suma(stringtoint(dato1), stringtoint(dato2));
	mensaje = "Resultado: " + inttostring(resultado);
	cout << mensaje;
	cout <<recibir(nuevo_socket);					// Aceptar el gracias.... ¿Porque aparece al final? NO SE
	enviar(nuevo_socket, mensaje);					// enviar resultado (4)
}

void servicio2()
{
	string palabrarec, mensaje;
    int res;
	cout <<"... Contar Caracteres ...\n";
	palabrarec = recibir(nuevo_socket);
	cout <<"El cliente dice: " <<palabrarec <<endl;
    enviar(nuevo_socket, "Palabra recibida");
    res = palabrarec.size();
    mensaje = "Tiene: " + inttostring(res) + " caracteres\n";
    cout << mensaje;
    cout <<recibir(nuevo_socket);
    enviar(nuevo_socket, mensaje);
}

void servicio3()
{
	string mensaje, numrec;
	int i, res;
	cout <<"... Multiplicacion ...\n";
	numrec = recibir(nuevo_socket);
	cout <<"Multiplos del  " << numrec <<endl;
	for (i = 1; i <= 10; i++)
	{
		res = i * stringtoint(numrec);
		mensaje = numrec + " * " + inttostring(i) + " = " + inttostring(res); 
		cout << mensaje;
		enviar(nuevo_socket, mensaje);
		cout <<recibir(nuevo_socket);
	}
}
void servicio4()
{	
	string dato, mensaje;
	char cad[MAXLONGITUD];
	double suma = 0, n = 0, promedio;
	dato = recibir(nuevo_socket);
	cout <<"Numeros enviados: " << dato <<endl;
	strcpy(cad, dato.c_str());
	char *numero;
	numero = strtok (cad, "-");
	while(numero != NULL)
	{
		cout << "Numero: " <<numero <<endl;	
		if (numero != NULL)
		{	
			suma = suma + atoi(numero);
			n++;
			cout <<"suma: " <<suma <<endl;
		}
		numero = strtok (NULL, "-");	
	}
	promedio = suma/(n-1);	
	mensaje = "Promedio: " + doubletostring(promedio);
	cout <<mensaje <<endl;
	enviar(nuevo_socket, mensaje);			// Enviar el promedio
}

// Funcion que suma dos numeros
int suma (int a, int b)
{
    int res = a + b;
    return res;
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


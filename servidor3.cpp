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
void verIP(struct sockaddr_in, int);
int nueva_cx(int);
string recibir(int);
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
void verIP (struct sockaddr_in c, int sock)
{
    char*  dire;
    dire = inet_ntoa(c.sin_addr);
    cout <<"Direccion del cliente: " <<dire <<endl;
    enviar(sock, "...Bienvenido al servidor...\n\n");
}

// Funcion que envia mensajes al cliente (tiene sobrecarga)
// digamos dos constructores

void enviar (int sock)
{
    send(sock, "Breve...\n", 10, 0);
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

int main ()
{
    inicializar();
    while(1)
    {
    	cout <<"\n!!!... SERVIDOR 2.0 ....!!!\n\n";
        nuevo_socket = nueva_cx(nosocket);
        verIP(cliente, nuevo_socket);
        //enviar(nuevo_socket, "bien o no");
        //cout <<"En espera del primer numero... \n";
        cout <<"Cliente dice: " << recibir(nuevo_socket) <<endl;
        enviar(nuevo_socket);
        //cout <<"Cliente dice: " << recibir(nuevo_socket) <<endl;
        /*
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
        */
        close(nuevo_socket);
    } // fin del while (1)
    return 0;
}

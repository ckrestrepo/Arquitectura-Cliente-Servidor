#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"

using namespace std;

#define MIPUERTO 9999   //pto de conexion para usuarios
#define MIIP "127.0.0.1"
#define ENCOLA 10       //numero conexiones permitidas
#define MAXLONGITUD 128

int main ()
{
    int nosocket, nuevo_socket;
    struct sockaddr_in dirservidor;     // info sobre mi direccion   
    struct sockaddr_in cliente;         // info sobre conexiones entrantes
    socklen_t señal_tam;                // tamaño de  la señal
    
    // llamada al socket
    nosocket = socket(AF_INET, SOCK_STREAM, 0);
    if (noscoket == -1)
    {
        cout<<"Error de socket"<<endl;
        exit(-1);
    }
    
    //Configuracion del servidor
    
    dirservidor.sin_family = AF_INET;
    dirservidor.sin_port = htons(MIPUERTO);
    //dirservidor.sin_addr.s_addr=INADDR_ANY;
    //inet_aton("localhost", &(dirservidor.sin_addr));
    //dirservidor.sin_addr.s_addr = inet_addr(MIIP);
    dirservidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&dirservidor.sin_zero),8);
    //memset(&dirservidor.sin_zero), 0, 8);
    
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
    cout <<"nosotros: " <<nosocket <<endl;
    while(1)
    {
        /*
        señal_tam = sizeof(struct sockaddr_in);
        nuevo_socket = accept(nosocket, (struct sockaddr*)&cliente, &señal_tam);
        if (nuevo_socket == -1)
        {
            cout <<"error en accept()" <<endl;
            exit(-1);
        }
        */
    }
}

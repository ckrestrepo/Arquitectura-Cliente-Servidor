/*
cliente.cpp
*/
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "sstream"

using namespace std;

#define MIPUERTO 9090
#define MAXLONGITUD 128

int nosocket, numbytes, opcion;
//char msj[MAXLONGITUD];   
char const host_servidor[]="127.0.0.1";
struct hostent *host_entrante;
struct sockaddr_in servidor;
  
void iniciar();
void conectar();
string recibir(int);
void enviar(int, string);

int stringtoint(string);
string inttostring(int);
void mostrar_menu();

int main()
{
  
   iniciar();
   conectar();  
   cout<<"Mensaje: "<<recibir(nosocket)<<endl;
   //Nuevo mensaje al servidor
   int a, b, mul, i;
   double promedio;
   string palabra;
   do
   {
    mostrar_menu();
    enviar(nosocket, inttostring(opcion));  // Enviar opcion (1)
    cout << recibir(nosocket) <<endl;   
    switch(opcion)
    {
      case 1: 
      {
        cout <<"Digite el primer numero: ";
        cin >> a;
        enviar(nosocket, inttostring(a));   // Enviar primer digito (2)
        cout <<recibir(nosocket) <<endl;    // Recibido por parte del servidor del primer digito (3)
        cout <<"Digite el segundo numero: ";
        cin >> b;
        enviar(nosocket, inttostring(b));   // Enviar segundo dato (3)
        cout <<recibir(nosocket) <<endl;    // Recibido por parte del servidor del segundo digito (4)
        enviar(nosocket, "\nGracias\n");
        cout <<recibir(nosocket) <<endl;    // Recibido del resultado (4)
        break;
    	}
      case 2:
      {
        cout <<"Digite la palabra: ";
        cin >> palabra;
        enviar(nosocket, palabra);
        cout <<recibir(nosocket) <<endl;
        enviar(nosocket, "\nGracias\n");
        cout <<recibir(nosocket) <<endl;    
        break;
      }

      case 0:
      {
        enviar(nosocket, "\nHe salido del servidor...\n");
        cout << recibir(nosocket) <<endl;
        break;
      }
      default:
        cout <<"Opcion incorrecta... Imbecil\n";
        break;
    }

/*
    msj[0]='\0';
    cout<<"Digite numero: ";
    cin>>msjenv;
    enviar(nosocket,msjenv);
    cout<<"Mensaje: "<<recibir(nosocket)<<endl;*/
   }while(opcion != 0);
   close(nosocket);
}



void mostrar_menu()
{
  cout <<"\n...Menu de Opciones...\n";
  cout <<"1. Sumar\n";
  cout <<"2. Contar caracteres\n";
  cout <<"0. Salir...\n";
  cout <<"Digite una opcion: ";
  cin >> opcion;
}

void iniciar(){
host_entrante=gethostbyname(host_servidor);
  if (host_entrante==NULL)
  {       
      /* llamada a gethostbyname() */
      cout<<"Error al recibir host "<< endl;
      exit(-1);
   }

   nosocket=socket(AF_INET, SOCK_STREAM, 0);
   if(nosocket==-1)
   {
     cout<<"Error en conexion 1. "<<endl; 
     exit(-1);
   }

   servidor.sin_family= AF_INET;
   servidor.sin_port = htons(MIPUERTO);
   servidor.sin_addr = *((struct in_addr *)host_entrante->h_addr);
   //he pasa la informacion de *he hacia h_addr
   bzero(&(servidor.sin_zero),8);
  }

void conectar(){
    
   if (connect(nosocket, (struct sockaddr *)&servidor, sizeof(struct sockaddr))==-1)
   {
     cout<<"Error en conexion 2"<<endl;
     exit(-1);
   }
}

string recibir(int s){
  char msj[MAXLONGITUD];
  string smsj="";
  int numbytes=recv(s,msj,MAXLONGITUD,0);
      if (numbytes==-1)
      {
       cout<<"Error al recibir. "<< endl;
      }
      if(numbytes==0)
      {
       cout<<"Conexion finalizada"<<endl;
      }
      if(numbytes>0)
      {
        msj[numbytes]='\0';
        //cout<<"Mensaje del cliente: "<<msj<<endl;
        smsj=(string)msj;
      }
     return smsj;
}

void enviar(int s, string cad){
  char msj[MAXLONGITUD];
  strcpy(msj, cad.c_str());
  send(s, msj, cad.size(),0); 
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








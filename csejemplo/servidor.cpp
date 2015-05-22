#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "pthread.h"
#include "sstream"

using namespace std;

#define MIPUERTO 9090  //pto de conexion para usuarios
#define MIIP "127.0.0.1"
#define ENCOLA 10 //no conexiones permitidas
#define MAXLONGITUD 128

int nosocket, nuevo_socket;
struct sockaddr_in dirservidor;
struct sockaddr_in cliente; 
socklen_t senal_tam; 

void iniciar();
int nueva_cx(int);
void verIp(struct sockaddr_in, int);
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

/***   SECCION DE HILOS    ****/
void *hcliente (void*);


int main(int argc, char *argv[])
{
  pthread_t hilos[5];
  int iret, iret2;
  iniciar();
  cout <<"\n...Bienvenido al servidor...\n";
  while(1) 
  {
    nuevo_socket=nueva_cx(nosocket);
    verIp(cliente, nuevo_socket);
    //Procesamiento del mensaje recibido 
    for (int i = 0; i < 5; ++i)
    {
        if (pthread_create (&hilos[i] , NULL , hcliente , &nuevo_socket))
        {
          cout <<"\nIniciando el hilo ["<<(i+1)<<"]...\n";
          pthread_join(hilos[i], NULL);
          close(nuevo_socket); 
        }    
    }
  }//fin while
   return 0;
}




void *hcliente (void* ptro)
{
  int sock = *((int*)ptro);
  string valoropc;
  int opcion;
    do
    {     
      valoropc = recibir(nuevo_socket);
      cout<<"Opcion del cliente: "<< valoropc <<endl;
      opcion = stringtoint(valoropc);
      enviar(nuevo_socket, "Opcion recibida");
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
          enviar(nuevo_socket, "\nAdios\n"); //Enviar en caso de que sea la opcion 3
          break;
        default:
          cout <<"El cliente digito una opcion incorrecta..\n";
          break;
      }
    }while(opcion != 0);
}

void servicio1()
{
  string dato1, dato2, mensaje;
  int resultado;
  cout <<"... Suma de dos digitos ...\n";
  dato1 = recibir(nuevo_socket);          // Recibio digito a (2)
  cout << "Primer digito: " << dato1 <<endl;
  enviar(nuevo_socket, "Numero 1 recibido\n");  // enviar informe de numero recibido (3)
  dato2 = recibir(nuevo_socket);          // Recibio digito b (3)
  cout << "Segundo digito: " << dato2 <<endl;
  enviar(nuevo_socket, "Numero 2 recibido\n");  // enviar informe de numero recibido (4)
  resultado = suma(stringtoint(dato1), stringtoint(dato2));
  mensaje = "Resultado: " + inttostring(resultado);
  cout << mensaje;
  cout <<recibir(nuevo_socket);         // Aceptar el gracias.... ¿Porque aparece al final? NO SE
  enviar(nuevo_socket, mensaje);          // enviar resultado (4)
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

int suma (int a, int b)
{
    int res = a + b;
    return res;
}



void iniciar(){
  nosocket=socket(AF_INET,SOCK_STREAM,0);
  if(nosocket==-1)
  {
    cout<<"Error en socket."<<endl;
    exit(-1);
  }
  
  dirservidor.sin_family=AF_INET;
  dirservidor.sin_port=htons(MIPUERTO);
  dirservidor.sin_addr.s_addr = inet_addr(MIIP);
  bzero(&(dirservidor.sin_zero),8);
  
  if(bind(nosocket,(struct sockaddr*)&dirservidor,sizeof(struct sockaddr))==-1) {
      cout<< "error en bind() " << endl;
      exit(-1);
   }  

   if(listen(nosocket,ENCOLA) == -1) 
   {  
      cout<<"error en listen() "<<endl;
      exit(-1);
   }

   //cout<<"nosocket: "<<nosocket<<endl;
}

int nueva_cx(int s){
  senal_tam=sizeof(struct sockaddr_in);
  int ns = accept(s,(struct sockaddr *)&cliente,&senal_tam);
      if (nuevo_socket==-1) 
      {
         cout<<"error en accept() " << endl;
         exit(-1);
      }
  return ns;    
}

void verIp(struct sockaddr_in c, int s){
  char *dire;
  dire=inet_ntoa(c.sin_addr);
  cout<<"Direccion del cliente: "<<dire<<endl;
  enviar(s, "Bienvenido al servidor.\n" );
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

void enviar(int s){
   send(s, "recibido...\n", 12 ,0); 
}

void enviar(int s, string cad){
  char msj[MAXLONGITUD];
  strcpy(msj, cad.c_str());
  send(s, msj, cad.size(),0); 
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

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
using namespace std;

#define MIIP "127.0.0.1"
#define MIPUERTO 9999
#define MAXLON 256
#define MAXCAD 100

void inisocket();
void iniservidor();
void recibir (int);
string recibir (int, int);
void enviar (int, string);
void enviar (int);
void Responder(int);
string enviarInfo (int, string);
int LeerMenu();
void Servicio1();
void Servicio2();
void Servicio3();
void Servicio5();

int nosocket;
char msj[MAXLON];   
char const host_servidor[]="127.0.0.1";
struct hostent *host_entrante;
struct sockaddr_in servidor;
string mensaje="";

int main()
{

  inisocket();
  iniservidor();
 
  int env;  
  recibir(nosocket);
  //enviar(nosocket,"Cliente conectado.\n");
  cout<<"Mensaje:"<<mensaje<<endl;
  
  string msjenv;
  //Gestion de servicio
   int op=0;
   do
   {
    op=LeerMenu();
    cout<<"Opcion: "<<op<<endl;
    
    //int op=1;
    if (op==1){ Servicio1(); }
    if (op==2){ Servicio2(); }
    if (op==3){ Servicio3(); }
    if (op==5){ Servicio5(); }
   }while(op!=6);
   
   close(nosocket);

}

int LeerMenu(){
    int op=0;
    do{
    cout<<"Menu"<<endl;
    cout<<"1 Sumar"<<endl;
    cout<<"2 Restar"<<endl;
    cout<<"3 Multiplicar"<<endl;
    cout<<"4 Divir"<<endl;
    cout<<"5 Sumatoria"<<endl;
    cout<<"6 Salir"<<endl;
    cin>>op;
    }while(op<0 || op>6);
    return op;
}

//Servicios

void Servicio1(){
   
   string cadcomp="1";
   enviar(nosocket,cadcomp); 
   recibir(nosocket);

   string v1,v2;
   
   cout<<"Ingrese numero 1: ";
   cin>>v1;
   enviar(nosocket,v1);
   recibir(nosocket);

   cout<<"Ingrese numero 2: ";
   cin>>v2;
   enviar(nosocket,v2);
   recibir(nosocket);

   recibir(nosocket);
   cout<<"Suma: "<<mensaje<<endl;
}

void Servicio2(){
  string cadcomp="2";
  enviar(nosocket,cadcomp);
}

void Servicio3(){
  string cadcomp="3";
  enviar(nosocket,cadcomp);  
  
}

void Servicio5(){
  string cadcomp="5";
  enviar(nosocket,cadcomp);
  recibir(nosocket);

  string liminf, limsup;
  cout<<"Sumatoria (2i)X+3"<<endl;
  cout<<"ingrese lim inferior: ";
  cin>>liminf;
  enviar(nosocket,liminf);
  recibir(nosocket);

  cout<<"ingrese lim superior: ";
  cin>>limsup;
  enviar(nosocket,limsup);
  recibir(nosocket);

  //Recibo de respuesta
  do{
    recibir(nosocket);
    cout<<mensaje<<endl;
    Responder(nosocket);
  }while(mensaje!="fin");
  
}

void inisocket()
{
  host_entrante=gethostbyname(host_servidor);
  if (host_entrante==NULL)
  {   
      cout<<"Error al recibir host "<< endl;
      exit(-1);
   }

   nosocket=socket(AF_INET, SOCK_STREAM, 0);
   if(nosocket==-1)
   {
     cout<<"Error en conexion 1. "<<endl; 
     exit(-1);
   }
}

void iniservidor()
{
   servidor.sin_family= AF_INET;
   servidor.sin_port = htons(MIPUERTO);
   servidor.sin_addr = *((struct in_addr *)host_entrante->h_addr);   
   bzero(&(servidor.sin_zero),8);
      
   if (connect(nosocket, (struct sockaddr *)&servidor, sizeof(struct sockaddr))==-1)
   {
     cout<<"Error en conexion 2"<<endl;
     exit(-1);
   }
}

void recibir (int s){
      int nb;
      char msjrec[MAXLON];
      nb=recv(s,msjrec,MAXLON,0);
      if (nb==-1)
      {
       cout<<"Error al recibir. "<< endl;
       exit(-1);
      }
      if (nb==0)
      {
        cout<<"Conexion finalizada"<<endl;
        close(s);
      }
      if(nb>0)
      {
       msjrec[nb]='\0';
       mensaje=(string) msjrec;
       //cout<<"Mensaje: "<<mensaje<<endl;
       
      }
}

string recibir (int s, int vc){
      int nb;
      string msjnuevo="vacio";
      char msjrec[MAXLON];
      nb=recv(s,msjrec,MAXLON,0);
      if (nb==-1)
      {
       cout<<"Error al recibir. "<< endl;
       exit(-1);
      }
      if (nb==0)
      {
        cout<<"Conexion finalizada"<<endl;
        close(s);
      }
      if(nb>0)
      {
       msjrec[nb]='\0';
       //cout<<"Mensaje: "<<msjrec<<endl;
       msjnuevo=(string)msjrec;
      }
      return msjnuevo;
}


string enviarInfo (int s, string campo){
   char msjenv[MAXLON];  
   cout<<"Enviar "<< campo <<":" ;
   cin.getline(msjenv,MAXLON,'\n');
   mensaje=(string) msjenv;
   send(s,msjenv,mensaje.size(),0);
   return mensaje;
}

void enviar (int s, string msj){
  char msjenv[MAXLON];  
  strcpy(msjenv,msj.c_str());
  send(s,msjenv,msj.size(),0);
}

void enviar (int s){
   char msjenv[MAXLON];  
   cout<<"Enviar mensaje:" ;
   cin.getline(msjenv,MAXLON,'\n');
   mensaje=(string) msjenv;
   send(s,msjenv,mensaje.size(),0);
}

void Responder(int ns){
  send(ns,"Recibido.\n",8,0);
}
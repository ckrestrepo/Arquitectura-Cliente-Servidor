#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <wait.h>
#include <sstream>
#include <list>
using namespace std;

#define MIPUERTO 9999
#define MIIP "127.0.0.1"
#define MAXLON 256
#define ENCOLA 10

void inisocket();
void iniservidor();
void inienlace();
void Enviar(int, struct sockaddr_in);
int Recibir(int);
void Responder(int);
void Responder(int,string);
list<string> leerlistatuberia(int); 
string leertuberia(int);
string Acad(int);
int Servicio1(int);
int Servicio5(int);

struct sockaddr_in cliente;
struct sockaddr_in dirservidor;

int nosocket, nuevo;
socklen_t lon;
string mensaje;

int main ()
{
  inisocket();
  iniservidor();
  inienlace();
  int fin=1;
  do
  {
    lon=sizeof(struct sockaddr_in);
    nuevo = accept(nosocket, (struct sockaddr *)&cliente,&lon);
    if (nuevo==-1){
      cout<<"Error en accept"<<endl; }
    else{
      pid_t idProceso;
      int estadoHijo;

      if((idProceso = fork())==-1){
         perror ("No se puede crear proceso");
	 exit (-1);
      }
      //hijo
      if (idProceso == 0){

        int estado=0;
 	cout<<"Id proceso: "<<getpid()<<endl;
        int nosesion=getpid();
        do{
         estado=Recibir(nuevo);
         Responder(nuevo);
         cout<<"Servicio: "<<mensaje<<endl;
         //Se recibe id de servicio
	 if(mensaje=="1"){       
	   estado=Servicio1(nosesion);
         }

	 if(mensaje=="5"){
	   estado=Servicio5(nosesion);
         }
       
       }while((estado!=33)&&(estado!=-1));
        exit (33);
      }
      //padre
      if (idProceso > 0){
        Enviar(nuevo, cliente);
      }
    }//fin si accept

  }while(fin==1);

  return(0);
}

int Servicio1(int sesion){
    string cadena="/home/alterego/Ejercicios/cs/procesos/./sumadorparam02 ";
    int estadoint=0;	
    int contador=0;
    do{
      estadoint=Recibir(nuevo);
      cadena= cadena + " " + mensaje ;
      Responder(nuevo);
      contador++;
    }while ((estadoint!=33)&&(contador<2));

    if(estadoint!=33){
     cadena= cadena + " " + Acad(sesion) ;
     cout<<"cadena: "<<cadena<<endl;
     char comando[cadena.size()];
     strcpy(comando,cadena.c_str())  ;
     system(comando);
     string r=leertuberia(sesion);
     Responder(nuevo,r);
    }
    cout<<"Fin servicio 1"<<endl;
    return estadoint;
}

int Servicio5(int sesion){
  string cadena="/home/alterego/Ejercicios/cs/procesos/./sumatoriaparam01";
  int estadoint=0;	
  int contador=0;
  do{
     estadoint=Recibir(nuevo);
     cadena= cadena + " " + mensaje ;
     Responder(nuevo);
     contador++;
  }while ((estadoint!=33)&&(contador<2));
  
  if(estadoint!=33){
     //Invocacion de servicio
     cadena= cadena + " " + Acad(sesion) ;
     cout<<"cadena: "<<cadena<<endl;
     char comando[cadena.size()];
     strcpy(comando,cadena.c_str())  ;
     system(comando);

     //Respuesta del servicio
     list<string>r=leerlistatuberia(sesion);
     list<string>::iterator it;
     string vals;
     for(it=r.begin();it!=r.end();it++){
       vals=*it; 
       Responder(nuevo,vals); 
       Recibir(nuevo);
     }
     Responder(nuevo,"fin");
     Recibir(nuevo);
    }
    cout<<"Fin servicio 5"<<endl;
    return estadoint;
}

string leertuberia(int sesion){
  //nombramiento del archivo
  string strarchivo="arsuma" + Acad(sesion);
  char nomar[strarchivo.size()];
  strcpy(nomar,strarchivo.c_str());

  char memoria[80];
  string respuesta="";  
  FILE *archivo;
  archivo = fopen(nomar,"rt");
  while (!feof(archivo)) {
    fgets(memoria,80,archivo);
    respuesta=(string)memoria;
  }
  fflush(archivo);
  fclose(archivo);
  cout<<"Respuesta: "<<respuesta<<endl;
  return respuesta;
}

list<string> leerlistatuberia(int sesion){
  list<string> res;
  //nombramiento del archivo
  string strarchivo="arsuma" + Acad(sesion);
  char nomar[strarchivo.size()];
  strcpy(nomar,strarchivo.c_str());

  char memoria[80];
  string respuesta="";  
  FILE *archivo;
  archivo = fopen(nomar,"rt");
  while (!feof(archivo)) {
    fgets(memoria,80,archivo);
    respuesta=(string)memoria;
    res.push_back(respuesta);
  }
  fflush(archivo);
  fclose(archivo);
//   cout<<"Retornando respuesta: "<<respuesta<<endl;
  return res;
}

void inisocket()
{
  nosocket=socket(AF_INET,SOCK_STREAM,0);
  if(nosocket==-1)
  {
    cout<<"Error en socket."<<endl;
    exit(-1);
  }
}

void iniservidor()
{
  dirservidor.sin_family=AF_INET;
  dirservidor.sin_port=htons(MIPUERTO);
  dirservidor.sin_addr.s_addr = inet_addr(MIIP); 
  bzero(&(dirservidor.sin_zero),8);
}

void inienlace()
{
 if(bind(nosocket,(struct sockaddr*)&dirservidor,sizeof(struct sockaddr))==-1) 
  {
     cout<< "Error en bind() " << endl;
     exit(-1);
  }  

  if(listen(nosocket,ENCOLA) == -1) 
  {  
     cout<<"Error en listen() "<<endl;
     exit(-1);
  }
}

int  Recibir(int ns){
   int nb, salida;
   char msj[MAXLON];
   nb=recv(ns,msj,MAXLON,0);
   if (nb==-1){
    cout<<"Error al recibir. "<< endl; salida=-1; }
   if (nb==0){
    cout<<"Conexion finalizada"<<endl;
    close(ns);
    salida=33;
   }
   if(nb>0){
    msj[nb]='\0';
    cout<<"Mensaje cliente"<<ns<<": "<<msj<<endl;
    mensaje=(string)msj;
    salida=1;
   }
   return salida;
}

void Enviar(int ns, struct sockaddr_in cl){
  char *dire;
  dire=inet_ntoa(cl.sin_addr);
  send(ns,"Bienvenido al servidor.\n",22,0); 
  cout<<"Direccion entrante: "<<dire<<endl;
}

void Responder(int ns){
  send(ns,"Recibido.\n",8,0);
}

void Responder(int ns, string m){
  char msj[MAXLON];
  strcpy(msj,m.c_str());
  send(ns,msj,m.size(),0);
}

string Acad(int val){
   std::stringstream valcad;
   valcad<<val;
   return valcad.str();
}

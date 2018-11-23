#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>                                                        
#include <termios.h>       
#include <sys/ioctl.h> 
#include <sys/time.h>
#include <time.h> 
#include <getopt.h> 
#include <sqlite3.h> 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mail.h"

 
#define BAUDRATE B9600                                                
//#define MODEMDEVICE "/dev/ttyS0"        //Conexió IGEP - Arduino
#define MODEMDEVICE "/dev/ttyACM0"         //Conexió directa PC(Linux) - Arduino                                   
#define _POSIX_SOURCE 1 /* POSIX compliant source */                       
                                                           
struct termios oldtio,newtio;                                            
sqlite3 *db;
char *zErrMsg = 0;
int rc;
//configuració de la comunicació sèrie
static int callback(void *NotUsed, int argc, char **argv, char 
**azColName){
int i;
	for(i=0; i<argc; i++){
	
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	
	}
	
	printf("\n");
return 0;
}

int sqlite(char *ordre){

	rc = sqlite3_exec(db, ordre, callback, 0, &zErrMsg);

	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

return 0;
}

int	ConfigurarSerie(void)
{
	int fd;                                                           


	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY );                             
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }                            

	tcgetattr(fd,&oldtio); /* save current port settings */                 

	bzero(&newtio, sizeof(newtio));                                         
	//newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;             
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;             
	newtio.c_iflag = IGNPAR;                                                
	newtio.c_oflag = 0;                                                     

	/* set input mode (non-canonical, no echo,...) */                       
	newtio.c_lflag = 0;                                                     

	newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */         
	newtio.c_cc[VMIN]     = 1;   /* blocking read until 1 chars received */ 

	tcflush(fd, TCIFLUSH);                                                  
	tcsetattr(fd,TCSANOW,&newtio);
	
		
 	sleep(3); //Per donar temps a que l'Arduino es recuperi del RESET
		
	return fd;
}  

//Funció per envir missatges a l'arduino              
void enviar(char *missatge, int res, int fd)
{                                                       
	
	res = write(fd,missatge,strlen(missatge));

	if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	
}

 //Funció per poder rebere missatges de l'arduino 
void rebre (char *buf, int fd, int temps)
{
	int res=0;
	int j=0;
	int bytes=0; 
	                                      
	
	//Mirem quans bytes ens ha enviat l'Arduino
  	ioctl(fd, FIONREAD, &bytes);
	
	res=0;
	//Rebem
	j=0;
	do	
	{
		res = res + read(fd,&buf[j],1);
		j++;
		
	} 
	while (buf[j-1] != 'Z');//Llegeix fins trobar la Z fi de trama
	
	
		
}

//Funció similar a enviar, pero en aquest cas només s'utilitza per encendre el led13 d'Arduino
void enviarled(char *missatge, int res, int fd)
{    
	                                           
	if (missatge[4]=='0')
	{
		missatge="AS131Z";//missatge per encendre led13
		res = write(fd,missatge,strlen(missatge));
	
		if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	}
	else
	{
		missatge="AS130Z";//misatge per apagar el led13
		res = write(fd,missatge,strlen(missatge));
		
		if (res <0) {tcsetattr(fd,TCSANOW,&oldtio); perror(MODEMDEVICE); exit(-1); }
	}
	
}

//Funció similar a rebre, pero en aquest cas només s'utilitza per rebre els missatges referetns al led13 d'Arduino
void rebreled (char *buf, int fd, int temps)
{
	int res=0;
	int j=0;
	int bytes=0;                                       
	
	//Mirem quans bytes ens ha enviat l'Arduino
  	ioctl(fd, FIONREAD, &bytes);
	
	res=0;
	//Rebem
	j=0;
	do	
	{
		res = res + read(fd,&buf[j],1);
		j++;
		
		
	} 
	while (buf[j-1] != 'Z');//Llegeix fins trobar la Z fi de trama

	
		
}

//Funció per tancar la comunicació sèrie
void TancarSerie(int fd)
{
	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);
}

int main(int argc, char *argv[]) {                                                              
   
    //Declaració variables funció main                                                                   
	int i=0, fd, res=0,m=1, lectures=0, pos=0, excestemp=0, up=0;                                                     
	float array[3600];
	float graus=0, maxim=0, minim=99;
	int comp=0;
	char buf[255];
	char missatge[255];
	char ordre[100];
	char hora[255];
	char *remitent=NULL;
	char *desti=NULL;
	char *dades=NULL;
	char cos_email[1024];
	int comparacio=0;
	int opt= 0;
    int temperatura = -1;
    int alarma=0;
	memset(buf,'\0',256);
	fd = ConfigurarSerie();	
	time_t temps;

        //Capturem el temps amb la funcio time(time_t *t);
	temps = time(NULL);
	// Defineix punter a una estructura tm
    struct tm * p_data;

	//Funcion localtime() per traduir segons UTC a la hora:minuts:segons de la hora local
	//struct tm *localtime(const time_t *timep);
    p_data = localtime(&temps);
	// Enviar el missatge 1, possada en marxa
    struct tm {
		int tm_sec;         /* seconds */
		int tm_min;         /* minutes */
		int tm_hour;        /* hours */
		int tm_mday;        /* day of the month */
		int tm_mon;         /* month */
		int tm_year;        /* year */
		int tm_wday;        /* day of the week */
		int tm_yday;        /* day in the year */
		int tm_isdst;       /* daylight saving time */
};
    static struct option long_options[] = {
        {"temperatura",   required_argument, 0,  't' },
        {"basedades",   required_argument, 0,  'c' },
        {"remitent",   required_argument, 0,  'r' },
        {"desti",   required_argument, 0,  'd' },
        {0,           0,                 0,  0   }
    };

    int long_index =0;

    while ((opt = getopt_long(argc, argv,"t:r:c:d:", 
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 't' : temperatura = atoi(optarg);
             printf("La temperatura és: %i\n",temperatura);
      		 break;
			 case 'r':
			 remitent=(optarg);
			 printf ("El remitent serà: %s\n", remitent);
			 break;
			 case 'c':
			 dades=(optarg);
			 printf ("La base de dades serà: %s\n", dades);
			 break;
			 case 'd':
			 desti=(optarg);
			 printf ("El destinatari serà: %s\n", desti);
			 break;
          default: printf("Si us plau, introdueix el paràmetre -t i -d per indicar la temperatura i la base de dades\n");
                 exit(EXIT_FAILURE);
        }
    }
    rc = sqlite3_open(dades, &db);
	if( rc ){
		fprintf(stderr, "No s'ha pogut obrir la base de dades: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	sprintf(ordre,"CREATE TABLE taula ( data DATETIME, temperatura FLOAT, estat INT)");
	sqlite(ordre);
	sprintf(ordre,"CREATE TABLE alarmes ( data DATETIME, temps float)");
	sqlite(ordre);
    if (temperatura == -1) {
       printf("Si us palu, introdueix el paràmetre -t o -temperatura seguit de la temperatura màxima");
        exit(EXIT_FAILURE);
    }

	sprintf(missatge,"AM1101Z");

	printf("%s\n",missatge);//es verfifica pel terminal el missatge enviat
	enviar(missatge, res, fd);
	
	rebre(buf, fd, 60);
	
	for (i = 0; i < res; i++)
	{
		printf("%c",missatge[i]);
	}
	printf("\n");
	printf("%s\n",buf);	//es verifica que s'ha rebut el missatge correcte de l'arduino

//es comproba que el missatge rebut ha estat el correcte 'AMOZ'	
if (strncmp(buf,"AM0Z",4)==0)
{
	comparacio=0;
	graus=0;	
	comp=0;
	
	//es realitza un do/while per fer un bucle que llegeixi dades constantment
	do
	{
       sprintf(hora,"%.02i/%.02i/%i %.02i:%.02i:%.02i",p_data->tm_mday,p_data->tm_mon,1900+p_data->tm_year, p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
		memset(buf,'\0',256);//es neteja el buf per poder llegir els valors correctament
		
		//cada vegada que comp sigui igual al temps establert per la consola es farà la comunicació de missatges
		if (comp==comparacio+10)
		{
			//S'envien i es reben els diferetns missatges amb l'arduino
			sprintf(ordre,"INSERT INTO taula VALUES ('%s', %f, %i)",hora,graus,up);
			sqlite(ordre);
			sprintf(missatge,"AS131Z");//encdre led13
			enviarled(missatge,res,fd);
			rebreled(buf, fd,10);
			sprintf(missatge,"ACZ");//confimació ok
			enviar(missatge,res,fd);
			memset(buf,'\0',256);
			rebre(buf, fd, 10);
			printf("%s\n",buf);//comprobació del missatge rebut
			comparacio=comp;
			graus=((buf[5]-'0')*10+(buf[6]-'0')+(buf[7]-'0')*0.1+(buf[8]-'0')*0.01);//es tradueixen els graus del missatge (string) a float
			printf("%.02f\n",graus);//temrpeatura amb deciamals per pantalla
			memset(buf,'\0',256);
			sprintf(missatge,"AS130Z");//apagar led 13
			enviarled(missatge, res, fd);
			rebreled(buf, fd, 10);			
			lectures++; //variable per saber el nombre de lectures fetes
			printf("%i\n",lectures);
			pos++; //posició de l'array circular
		}
			//si la posició de l'array circular arriba a 3600, es torna a 0
			if (pos==3600)
			{
				printf("%f",array[i]);
			pos=0;	
			}
			array[pos]=graus; //es guarda la temperatura a l'arrray circular 3600
		
	
			
		//Comprovació per encendre o apagar el ventilador
	
	if (alarma==0 && up==1)
	{
		alarma=comparacio+300;
	}
	if (alarma==comp){
	sprintf(ordre,"INSERT INTO alarmes VALUES ('%s', %i)",hora,comp);
	sqlite(ordre);
	alarma = 0;
	memset(cos_email,'\0',256);
	sprintf(cos_email,"Subject: Alarma\nFrom: %s\nTo: %s\n\nNo és possible controlar la temperatura\n\n",remitent,desti);
	enviar_mail(remitent,desti,cos_email);
	}
	if (up==0){
		if (graus >= temperatura)
		{
			sprintf(missatge,"AS051Z");
			enviar(missatge,res,fd);
			memset(buf,'\0',256);
			rebre(buf, fd, 60);
			memset(buf,'\0',256);
			excestemp=1; //protecció per a que no entri continuament al següent if
			up=1;//variable per no tornar a entrar
		}
	}	
	
		if (graus < temperatura && excestemp==1)
		{
			sprintf(missatge,"AS050Z");
			enviar(missatge,res,fd);
			memset(buf,'\0',256);
			rebre(buf, fd, 60);
			memset(buf,'\0',256);
			excestemp=0;
			up=0;
		}

		//comparem les adqusicions per guardar els màxims i mínims de temperatura
		if (maxim < graus)
			{
				maxim=graus;
				printf("El màxim és %f\n",maxim);
			}
			if (minim >graus)
			{
				minim=graus;
				printf("El mínim és %f\n",minim);
			}

			sleep(1);
			comp++; //s'incrementa la varibale comp cada segon(sleeep(1))->comptador
			
	} while (m==1);
	
}		 
	//cridem a la funció per tancar la comunicació sèrie                                                         
	TancarSerie(fd);
	sqlite3_close(db);
	return 0;
}

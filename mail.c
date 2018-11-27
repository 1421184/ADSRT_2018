#include "mail.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#define REQUEST_MSG_SIZE	1024
#define REPLY_MSG_SIZE		500
#define SERVER_PORT_NUM		25


int enviar_mail(char *remitent, char *desti, char *cos_email) {

	struct sockaddr_in	serverAddr;
	char	    serverName[] = "172.20.0.21"; //Adreça IP on està el client
	int			sockAddrSize;
	int			sFd;
	int 		result;
	char		buffer[REQUEST_MSG_SIZE];
	char		missatge0[] = "HELO AE2004\n";
	char		missatge1[] = "MAIL FROM: ";
	char		missatge2[] = "RCPT TO: ";
	char		missatge3[] = "DATA\n";
	char		missatge4[] = "QUIT\n";
	char 		missatge[REQUEST_MSG_SIZE];


        //El valor de retorn es a una variable de tipus timei_t, on posaràl temps en segons des de 1970-01-01 00:00:00 +0000 (UTC)
	
	// Defineix punter a una estructura tm

	//Funcion localtime() per traduir segons UTC a la hora:minuts:segons de la hora local
	//struct tm *localtime(const time_t *timep);
        //Es treu per pantalla el camp tm_sec de l'estructura temps, que són els segons de la hora actual

	/*Crear el socket*/
	sFd=socket(AF_INET,SOCK_STREAM,0);
	/*Construir l'adreça*/
	sockAddrSize = sizeof(struct sockaddr_in);
	bzero ((char *)&serverAddr, sockAddrSize); //Posar l'estructura a zero
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons (SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(serverName);
	/*Conexió*/
	result = connect (sFd, (struct sockaddr *) &serverAddr, sockAddrSize);
	if (result < 0)
	{
		printf("Error en establir la connexió\n");
		exit(-1);
	}
	printf("\nConnexió establerta amb el servidor: adreça %s, port %d\n",	inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);

	/*Enviar*/
	strcpy(buffer,missatge0); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	/*Enviar*/
	strcpy(buffer,missatge1);
	strcat(buffer,remitent);
	strcat(buffer,"\n");
	result = write(sFd, buffer, strlen(buffer));
	buffer[result]=0;
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	/*Enviar*/
	strcpy(buffer,missatge2); //Copiar missatge a buffer
	strcat(buffer,desti);
	strcat(buffer,"\n");
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	/*Enviar*/
	strcpy(buffer,missatge3); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	
	// -- Contruir el cos email

          	
	/*Enviar*/
	strcpy(buffer,cos_email);
	strcat(buffer,missatge);
	strcat(buffer,"\n.\n");
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);
	
	/*Rebre*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	result = read(sFd, buffer, REQUEST_MSG_SIZE);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);

	/*Enviar*/
	memset(buffer, 0, REQUEST_MSG_SIZE);
	strcpy(buffer,missatge4); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, buffer);

	
	close(sFd);
	return 0;
}


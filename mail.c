/***************************************************************************
                          main.c  -  client
                             -------------------
    begin                : lun feb  4 16:00:04 CET 2002
    copyright            : (C) 2002 by A. Moreno
    email                : amoreno@euss.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

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



 /************************
*
*
* tcpClient
*
*
*/
int main(int argc, char *argv[]){
	struct sockaddr_in	serverAddr;
	char	    serverName[] = "172.20.0.21"; //Adreça IP on està el client
	int			sockAddrSize;
	int			sFd;
	int 		result;
	char		buffer[256];
	char		desti;
	char		asumpte;
	char		missatge2[] = "RCPT TO: ";
	char		missatge4[1024];
        //Definim una variable de tipus time_t
	time_t temps;

        //Capturem el temps amb la funcio time(time_t *t);
	temps = time(NULL);
        //El valor de retorn es a una variable de tipus timei_t, on posaràl temps en segons des de 1970-01-01 00:00:00 +0000 (UTC)

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

	// Defineix punter a una estructura tm
        struct tm * p_data;

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
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	printf("Escriu la direcció de destí\n");	
	
	scanf("%c",&desti);
	
	strcat(missatge2, &desti);
	printf("Escriu l'asumpte del correu\n");
	scanf("%c",&asumpte);
	sprintf(missatge4,"Subject: %c\nFrom: 1457636@campus.euss.org\nTo: %c\nHola Salva\n\n Això es un mail de prova\n\n",desti, asumpte);
	
void mail(char *missatge, int res, int fd){	
	char		HELO[] = "HELO AE2004\n";
	char		missatge1[] = "MAIL FROM: 1421184@campus.euss.org\n";
	char		missatge3[] = "DATA\n";
	
	/*Enviar*/
	strcpy(buffer,HELO); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n", result, HELO);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	/*Enviar*/
	strcpy(buffer,missatge1); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	buffer[result]=0;
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge1);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);

	/*Enviar*/
	strcpy(buffer,missatge2); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge2);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	/*Enviar*/
	strcpy(buffer,missatge3); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge3);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	p_data = localtime( &temps );
            
    sprintf(missatge,"\n\nL'hora actual és: %i:%.02i:%.02i\n\nAdéu\n.\n", p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
	printf("%s",missatge);
	strcat(missatge4,missatge);
	/*Enviar*/
	strcpy(buffer,missatge4); //Copiar missatge a buffer
	result = write(sFd, buffer, strlen(buffer));
	printf("Missatge enviat a servidor(bytes %d): %s\n",	result, missatge4);

	/*Rebre*/
	result = read(sFd, buffer, 256);
	buffer[result]=0;
	printf("Missatge rebut del servidor(bytes %d): %s\n",	result, buffer);
	
	close(sFd);
}
	return 0;
	}

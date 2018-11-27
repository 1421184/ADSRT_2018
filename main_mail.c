#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "mail.h"
#include <sqlite3.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 /************************
*
*
* tcpClient
*
*
*/
                                         
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *valorsql;
static int callback(void *NotUsed, int argc, char **argv, char 
**azColName){
int i;
sprintf(valorsql,"%s",argv[0]);
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
int main(int argc, char *argv[]){
//int mail(char *remitent, char *desti, char *cos_email);
	char ordre[1000];
	char cos_email[1000];
	char hora[255];
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
	
	sprintf(hora,"%.02i/%.02i/%i %.02i:%.02i:%.02i",p_data->tm_mday,p_data->tm_mon,1900+p_data->tm_year, p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
	//Busca i insereix la primera data del día guardada al sqlite
	sprintf(ordre,"SELECT MIN(data) FROM taula WHERE data >= %s",hora); 
	sqlite(ordre);
	memset(cos_email,'\0',1000);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
    //Busca i insereix l'última data guardada al sqlite
	sprintf(ordre,"SELECT MAX(data) FROM taula WHERE data >= %s",hora); 
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
    sprintf(hora,"%.02i/%.02i/%i %.02i:%.02i:%.02i",p_data->tm_mday-1,p_data->tm_mon,1900+p_data->tm_year, p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
    //Busca i insereix la temperatura màxima durant el día actual
    sprintf(ordre,"SELECT MAX(temperatura) FROM taula WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
	//Busca i insereix la temperatura mínima durant el día actual
    sprintf(ordre,"SELECT MIN(temperatura) FROM taula WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
	//Busca i insereix la temperatura mitjana durant el día actual
    sprintf(ordre,"SELECT AVG(temperatura) FROM taula WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
    //Busca i insereix el temps de funcionament del ventilador durant el día actual
    sprintf(ordre,"SELECT MAX(temps) FROM alarmes WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
    //Busca i insereix el temps mitjà de funcionament del ventilador durant el día actual
    sprintf(ordre,"SELECT AVG(temps) FROM alarmes WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
      //Busca i insereix el temps mitjà de funcionament del ventilador durant el día actual
    sprintf(ordre,"SELECT AVG(temps) FROM alarmes WHERE data >= %s",hora);
    sqlite(ordre);
    strcat(cos_email,valorsql);
    strcat(cos_email,"\n");
	enviar_mail("1422047@campus.euss.org", "1421184@campus.euss.org", cos_email );
    
	return 0;
}

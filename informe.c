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
#include <getopt.h>
#include <string.h>
 /************************
*
* tcpClient
*
*
*/

sqlite3 * db;
char *zErrMsg;
int rc;
char valorsql[1000];
int alarmes;
char cos_alarmes[1000];
char valor_taula[1000];
int i;
static int callback(void *NotUsed, int argc, char **argv, char
**azColName)
{

	printf("callback: %s\n", argv[0]?argv[0]:"(((NULL)))");
	sprintf(valorsql, "%s", argv[0]?argv[0]:"(((NULL)))");
	for (i = 0; i < argc; i++) {

		sprintf(valor_taula, "%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		if (alarmes) {
		strcat(cos_alarmes, valor_taula);
		}
	}

return 0;
}
int sqlite(char *ordre)
{
	printf("ordre: %s\n", ordre);

	rc = sqlite3_exec(db, ordre, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

return 0;
}

int main(int argc, char *argv[])
{
//int mail(char *remitent, char *desti, char *cos_email);
	char ordre[1000];
	char cos_email[1000];
	char dia_anterior[255], dia_seguent[255];
	int opt = 0;
	char *remitent = NULL;
	char *desti = NULL;
	char *dades = NULL;
	time_t temps;

	if (rc) {
		fprintf(stderr, "No s'ha pogut obrir la base de dades: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	sprintf(ordre, "CREATE TABLE taula ( data DATETIME, temperatura FLOAT, estat INT)");
	sqlite(ordre);
	sprintf(ordre, "CREATE TABLE alarmes ( data DATETIME, temps float)");
	sqlite(ordre);
	//Capturem el temps amb la funcio time(time_t *t);
	temps = time(NULL);
	// Defineix punter a una estructura tm
	struct tm *p_data;

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
		{"basedades",   required_argument, 0,  'b' },
		{"remitent",   required_argument, 0,  'r' },
		{"desti",   required_argument, 0,  'd' },
		{0,           0,               0,  0   }
	};

	int long_index = 0;

	while ((opt = getopt_long(argc, argv, "r:b:d:",
				long_options, &long_index)) != -1) {
		switch (opt) {
		case 'r':
		remitent = (optarg);
		printf("El remitent serà: %s\n", remitent);
		break;
		case 'b':
		dades = (optarg);
		printf("La base de dades serà: %s\n", dades);
		break;
		case 'd':
		desti = (optarg);
		printf("El destinatari serà: %s\n", desti);
		break;
		default:
		printf("Si us plau, introdueix el paràmetre -t i -d per indicar la temperatura i la base de dades\n");
		exit(EXIT_FAILURE);
		}
	}
	rc = sqlite3_open(dades, &db);
	sprintf(dia_anterior, "%.02i/%.02i/%i %.02i:%.02i:%.02i", p_data->tm_mday-1, p_data->tm_mon, 1900+p_data->tm_year, p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
	sprintf(dia_seguent, "%.02i/%.02i/%i %.02i:%.02i:%.02i", p_data->tm_mday+1, p_data->tm_mon, 1900+p_data->tm_year, p_data->tm_hour, p_data->tm_min, p_data->tm_sec);
	//Busca i insereix la primera data del día guardada al sqlite
	sprintf(ordre, "SELECT MIN(data) FROM taula;");
	sqlite(ordre);
	memset(cos_email, '\0', 1000);
	sprintf(cos_email, "Subject: Informe diari\nFrom: %s\nTo: %s\n", remitent, desti);
	strcat(cos_email, "- La data i l'hora d'inici és: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, "\n\n");
	//Busca i insereix l'última data guardada al sqlite
	sprintf(ordre, "SELECT MAX(data) FROM taula");
	sqlite(ordre);
	strcat(cos_email, "- La data i l'hora final és: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, "\n\n");
	//Busca i insereix la temperatura màxima durant el día actual
	sprintf(ordre, "SELECT MAX(temperatura) FROM taula WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- La temperatura màxima ha estat: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, "ºC\n\n");
	//Busca i insereix la temperatura mínima durant el día actual
	sprintf(ordre, "SELECT MIN(temperatura) FROM taula WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- La temperatura mínima ha estat: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, "ºC\n\n");
	//Busca i insereix la temperatura mitjana durant el día actual
	sprintf(ordre, "SELECT AVG(temperatura) FROM taula WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- La temperatura mitjana ha estat: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, "ºC\n\n");
	//Busca i insereix el temps de funcionament del ventilador durant el día actual
	sprintf(ordre, "SELECT MAX(temps) FROM alarmes WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- El ventilador ha estat funcionant durant: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, " segons\n\n");
	//Busca i insereix el temps mitjà de funcionament del ventilador durant el día actual
	sprintf(ordre, "SELECT AVG(temps) FROM alarmes WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- El temps mitj que ha estat funcionant el ventilador ha estat de: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, " segons\n\n");
	//Busca i insereix el número de vegades que ha funcionat el ventilador durant el día actual
	sprintf(ordre, "SELECT COUNT(*) FROM alarmes WHERE data<'%s' AND data>'%s';", dia_seguent, dia_anterior);
	sqlite(ordre);
	strcat(cos_email, "- El ventilador ha funcionat: ");
	strcat(cos_email, valorsql);
	strcat(cos_email, " vegades\n\n");
	//Incorpora el llistat d'alarmes
	sprintf(ordre, "SELECT data,temps FROM alarmes;");
	alarmes = 1;
	sqlite(ordre);
	alarmes = 0;
	strcat(cos_email, "-El llistat d'alarmes és el següent:\n\n");
	strcat(cos_email, cos_alarmes);
	printf("%s\n", cos_email);
	enviar_mail(remitent, desti, cos_email);
	sqlite3_close(db);
	return 0;
}

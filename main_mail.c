#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include "mail.h"
#include <sqlite3.h> 
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
int main(int argc, char *argv[]){
//int mail(char *remitent, char *desti, char *cos_email);
	char ordre[100];
	char cos_email[] = "Subject: mail d'exemple\nFrom: 1422047@campus.euss.org\nTo: 1421184@campus.euss.org\n\nHola Salva\n\n Això es un mail de prova\n\n";
    
    sprintf(ordre,"INSERT INTO taula VALUES");
    
    enviar_mail("1422047@campus.euss.org", "1421184@campus.euss.org", cos_email );
    


	return 0;
}

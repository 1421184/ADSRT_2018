#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "mail.h"

 /************************
*
*
* tcpClient
*
*
*/

int main(int argc, char *argv[]){
//int mail(char *remitent, char *desti, char *cos_email);

	char cos_email[] = "Subject: mail d'exemple\nFrom: 1457636@campus.euss.org\nTo: 1422047@campus.euss.org\n\nHola Salva\n\n Això es un mail de prova\n\n";
     enviar_mail("1421184@campus.euss.org", "afontquerni@euss.es", cos_email );
    

	return 0;
	}
